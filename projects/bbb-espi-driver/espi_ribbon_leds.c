#include <linux/of_gpio.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>
#include "espi_driver.h"

#define ESPI_RIBBON_LED_DEV_MAJOR	303
#define RIBBON_LED_STATES_SIZE		17
static u8 *rb_led_st;
static u8 *rb_led_new_st;
static u64 lastUpdate = 0;

/*******************************************************************************
    ribbon leds functions
*******************************************************************************/
static ssize_t rbled_write( struct file *filp,
                            const char __user *buf_user,
                            size_t count,
                            loff_t *f_pos)
{
	char buf[count];
	ssize_t status = 0;
	u32 i;
	u8 val, led_id;
	u8 rot[] = {0,2,1,3};

	if (copy_from_user(buf, buf_user, count))
		return -EFAULT;

	for(i=0; (i+1) < count; i+=2) {
		led_id = buf[i];
		val = rot[buf[i+1] & 0x3];

		rb_led_new_st[RIBBON_LED_STATES_SIZE - 1 - led_id/4] &= ~(0x3 << ((led_id%4)*2));
		rb_led_new_st[RIBBON_LED_STATES_SIZE - 1 - led_id/4] |= val << ((led_id%4)*2);
	}
	status = count;
	return status;
}

static ssize_t rbled_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t status = 0;
	return status;
}

static s32 rbled_open(struct inode *inode, struct file *filp)
{
	s32 status = 0;
	nonseekable_open(inode, filp);
	return status;
}

static s32 rbled_release(struct inode *inode, struct file *filp)
{
	s32 status = 0;
	return status;
}

static const struct file_operations rbled_fops = {
		.owner = 	THIS_MODULE,
		.write = 	rbled_write,
		.read =		rbled_read,
		.open =		rbled_open,
		.release = 	rbled_release,
		.llseek = 	no_llseek,
};

static struct class *rbled_class;

s32 espi_driver_rb_leds_setup(struct espi_driver *sb)
{
	s32 i, ret;

	rb_led_st = kcalloc(RIBBON_LED_STATES_SIZE,sizeof(u8), GFP_KERNEL);
	if (!rb_led_st)
		return -ENOMEM;
	rb_led_new_st = kcalloc(RIBBON_LED_STATES_SIZE,sizeof(u8), GFP_KERNEL);
	if (!rb_led_new_st)
		return -ENOMEM;

	for(i=0; i<RIBBON_LED_STATES_SIZE; i++)
		rb_led_new_st[i] = rb_led_st[i] = 0;

	ret = register_chrdev(ESPI_RIBBON_LED_DEV_MAJOR, "spi", &rbled_fops);
	if (ret < 0)
		pr_err("%s: problem at register_chrdev\n", __func__);

	rbled_class = class_create(THIS_MODULE, "ribbon-led");
	if(IS_ERR(rbled_class))
		pr_err("%s: unable to create class\n", __func__);

	device_create(rbled_class, sb->dev, MKDEV(ESPI_RIBBON_LED_DEV_MAJOR, 0), sb, "ribbon_led");

	return 0;
}

s32 espi_driver_rb_leds_cleanup(struct espi_driver *sb)
{
	kfree(rb_led_st);
	kfree(rb_led_new_st);

	device_destroy(rbled_class, MKDEV(ESPI_RIBBON_LED_DEV_MAJOR, 0));
	class_destroy(rbled_class);
	unregister_chrdev(ESPI_RIBBON_LED_DEV_MAJOR, "spi");

	return 0;
}

void espi_driver_rb_leds_poll(struct espi_driver *p)
{
	struct spi_transfer xfer;
	u32 i, update = 0;
	u64 now = get_jiffies_64();
	u64 diff = now - lastUpdate;
	unsigned int diffMS = jiffies_to_msecs(diff);
	unsigned int interval = 250;

	extern int sck_hz;


	for(i=0; i<RIBBON_LED_STATES_SIZE; i++) {
		if(rb_led_st[i] ^ rb_led_new_st[i]) {
			rb_led_st[i] = rb_led_new_st[i];
			update = 1;
		}
	}

	update |= diffMS >= interval;

	if(update == 0)
		return;

	lastUpdate = now;

	xfer.tx_buf = rb_led_st;
	xfer.rx_buf = NULL;
	xfer.len = RIBBON_LED_STATES_SIZE;
	xfer.bits_per_word = 8;
	xfer.delay_usecs = 0;
	xfer.speed_hz = sck_hz;

	espi_driver_scs_select((struct espi_driver*)p, ESPI_RIBBON_LEDS_PORT, (struct espi_driver*)p->ribbon_leds_device);
	espi_driver_transfer(((struct espi_driver*)p)->spidev, &xfer);
	gpio_set_value(((struct espi_driver *)p)->gpio_sap, 0);
	gpio_set_value(((struct espi_driver *)p)->gpio_sap, 1);
	espi_driver_scs_select((struct espi_driver*)p, ESPI_RIBBON_LEDS_PORT, 0);
}

