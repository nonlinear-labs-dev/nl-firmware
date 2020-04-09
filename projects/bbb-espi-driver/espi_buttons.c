#include <linux/poll.h>
#include <linux/of_gpio.h>
#include "espi_driver.h"

#define ESPI_BUTTON_DEV_MAJOR		302
#define BUTTON_BYTES_GENERAL_PANELS	12
#define BUTTON_BYTES_CENTRAL_PANEL	3
#define BUTTON_BYTES_SOLED_PANEL	1
#define BUTTON_STATES_SIZE		16
static u8 *btn_sm1;
static u8 *btn_st;
#define BUTTON_BUFFER_SIZE		256
static u8 *button_buff;
static u32 btn_buff_head, btn_buff_tail;
static DEFINE_MUTEX(btn_buff_tail_lock);
static DECLARE_WAIT_QUEUE_HEAD(btn_wqueue);

static ssize_t buttons_fops_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t status = 0;
	u8 tmp;

	/* If in non-blocking mode and no data to read, return */
	if (filp->f_flags & O_NONBLOCK && btn_buff_head == btn_buff_tail)
		return -EAGAIN;

	/* Sleep until there is data to read */
	if ((status = wait_event_interruptible(btn_wqueue, btn_buff_head != btn_buff_tail)))
		return status;

	mutex_lock(&btn_buff_tail_lock);

	// xor 0x80, so we get 1 on btn down and 0 on btn up
	tmp = button_buff[btn_buff_tail] ^ 0x80;
	if (copy_to_user(buf, &tmp, 1) != 0)
		return -EFAULT;

	btn_buff_tail = (btn_buff_tail+1)%BUTTON_BUFFER_SIZE;
	mutex_unlock(&btn_buff_tail_lock);

	return 1;
}

static unsigned int buttons_fops_poll(struct file *filp, poll_table *wait)
{

	unsigned int mask = 0;
	poll_wait(filp, &btn_wqueue, wait);

	/* If there is data in buffer, reading is allowed
	 * Writing to buttons never makes sense, so dissallow */

	if (btn_buff_tail != btn_buff_head)
		mask |= POLLIN | POLLRDNORM;

	return mask;
}

static const struct file_operations buttons_fops = {
		.owner = 	THIS_MODULE,
		.read =		buttons_fops_read,
		.open =		nonseekable_open,
		.llseek = 	no_llseek,
		.poll =		buttons_fops_poll,
};

static struct class *buttons_class;

s32 espi_driver_buttons_setup(struct espi_driver *sb)
{
	s32 i, ret;

	btn_sm1 = kcalloc(BUTTON_STATES_SIZE,sizeof(u8), GFP_KERNEL);
	if (!btn_sm1)
		return -ENOMEM;

	btn_st = kcalloc(BUTTON_STATES_SIZE,sizeof(u8), GFP_KERNEL);
	if (!btn_st)
		return -ENOMEM;

	for(i=0; i<BUTTON_STATES_SIZE; i++)
		btn_sm1[i] = btn_st[i] = 0xFF;

	button_buff = kcalloc(BUTTON_BUFFER_SIZE,sizeof(u8), GFP_KERNEL);
	if (!button_buff)
		return -ENOMEM;

	btn_buff_head = btn_buff_tail = 0;

	ret = register_chrdev(ESPI_BUTTON_DEV_MAJOR, "spi", &buttons_fops);
	if (ret < 0)
		pr_err("%s: problem at register_chrdev\n", __func__);

	buttons_class = class_create(THIS_MODULE, "espi-button");
	if(IS_ERR(buttons_class))
		pr_err("%s: unable to create class\n", __func__);

	device_create(buttons_class, sb->dev, MKDEV(ESPI_BUTTON_DEV_MAJOR, 0), sb, "espi_buttons");

	return 0;
}

s32 espi_driver_buttons_cleanup(struct espi_driver *sb)
{
	kfree(btn_sm1);
	kfree(btn_st);
	kfree(button_buff);

	device_destroy(buttons_class, MKDEV(ESPI_BUTTON_DEV_MAJOR, 0));
	class_destroy(buttons_class);
	unregister_chrdev(ESPI_BUTTON_DEV_MAJOR, "spi");

	return 0;
}

void espi_driver_pollbuttons(struct espi_driver *p)
{
	struct spi_transfer xfer;
	u8 rx[BUTTON_STATES_SIZE];
	u8 i, j, xor, bit, btn_id;

	extern int sck_hz;

	xfer.tx_buf = NULL;
	xfer.rx_buf = rx;
	xfer.len = BUTTON_BYTES_GENERAL_PANELS;
	xfer.bits_per_word = 8;
	xfer.delay_usecs = 0;
	xfer.speed_hz = sck_hz;

	espi_driver_set_mode(((struct espi_driver*)p)->spidev, SPI_MODE_3);

	/** read general panels */
	espi_driver_scs_select((struct espi_driver*)p, ESPI_SELECTION_PANEL_PORT, ESPI_SELECTION_BUTTONS_DEVICE);
	gpio_set_value(((struct espi_driver *)p)->gpio_sap, 0);
	gpio_set_value(((struct espi_driver *)p)->gpio_sap, 1);
	espi_driver_transfer(((struct espi_driver*)p)->spidev, &xfer);
	espi_driver_scs_select((struct espi_driver*)p, ESPI_SELECTION_PANEL_PORT, 0);

	/** read central (big oled) panel */
	xfer.tx_buf = NULL;
	xfer.rx_buf = rx + BUTTON_BYTES_GENERAL_PANELS;
	xfer.len = BUTTON_BYTES_CENTRAL_PANEL;
	espi_driver_scs_select((struct espi_driver*)p, ESPI_EDIT_PANEL_PORT, ESPI_EDIT_BUTTONS_DEVICE);
	gpio_set_value(((struct espi_driver *)p)->gpio_sap, 0);
	gpio_set_value(((struct espi_driver *)p)->gpio_sap, 1);
	espi_driver_transfer(((struct espi_driver*)p)->spidev, &xfer);
	espi_driver_scs_select((struct espi_driver*)p, ESPI_EDIT_PANEL_PORT, 0);

	/** read small oled panel */
	xfer.tx_buf = NULL;
	xfer.rx_buf = rx + BUTTON_BYTES_GENERAL_PANELS + BUTTON_BYTES_CENTRAL_PANEL;
	xfer.len = BUTTON_BYTES_SOLED_PANEL;
	espi_driver_scs_select((struct espi_driver*)p, ESPI_PLAY_PANEL_PORT, (struct espi_driver*)p->play_buttons_device);
	gpio_set_value(((struct espi_driver *)p)->gpio_sap, 0);
	gpio_set_value(((struct espi_driver *)p)->gpio_sap, 1);
	espi_driver_transfer(((struct espi_driver*)p)->spidev, &xfer);
	espi_driver_scs_select((struct espi_driver*)p, ESPI_PLAY_PANEL_PORT, 0);

      	/***** MASKING ******/
	//rx[BUTTON_BYTES_GENERAL_PANELS + 1] |= 0x30;
	rx[BUTTON_BYTES_GENERAL_PANELS + BUTTON_BYTES_CENTRAL_PANEL] |= 0x0F;

	/** check read states */
	for(i=0; i < BUTTON_STATES_SIZE; i++) {
		xor = (btn_sm1[i] & rx[i] & (~btn_st[i])) | (~(btn_sm1[i] | rx[i]) & btn_st[i]);
		if(xor){
			for(j=0; j < 8; j++){
				bit = xor & (1<<j);
				if(bit & btn_st[i])	// change 1->0
					btn_id = i*8 + 7-j;
				else if(bit)		// change 0->1
					btn_id = (i*8 + 7-j) | (1<<7);
				else
					continue;

				btn_st[i] ^= bit;

				if(((btn_buff_head+1)%BUTTON_BUFFER_SIZE) != btn_buff_tail) {
					button_buff[btn_buff_head] = btn_id;
					btn_buff_head = (btn_buff_head+1)%BUTTON_BUFFER_SIZE;
					wake_up_interruptible(&btn_wqueue);
				}
				//printk("button-change = %x\n", btn_id);
			}
		}
		btn_sm1[i] = rx[i];
	}

	espi_driver_set_mode(((struct espi_driver*)p)->spidev, SPI_MODE_0);
}

