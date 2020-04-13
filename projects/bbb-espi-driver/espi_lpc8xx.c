#include <linux/poll.h>
#include "espi_driver.h"
#include "espi_fb.h"

/* Encoder stuff **************************************************************/
#define ESPI_ENCODER_DEV_MAJOR		308
static s8 encoder_delta;
static DECLARE_WAIT_QUEUE_HEAD(encoder_wqueue);

static u8 tmp_boled_reset = 0;
static u8 tmp_boled_12v = 0;

/*******************************************************************************
    encoder functions
*******************************************************************************/
static ssize_t encoder_fops_write(   struct file *filp,
                                    const char __user *buf,
                                    size_t count,
                                    loff_t *f_pos)
{
	ssize_t status = 0;

	if(buf[0] == 'a') {
		tmp_boled_reset = 1;
	}
	else if(buf[0] == 'b') {
		tmp_boled_12v = 1;
	}

	status = count;

	return status;
}

static ssize_t encoder_fops_read(    struct file *filp,
                                    char __user *buf,
                                    size_t count,
                                    loff_t *f_pos)
{
	ssize_t status = 0;

	/* If in non-blocking mode and no data to read, return */
	if (filp->f_flags & O_NONBLOCK && encoder_delta == 0)
		return -EAGAIN;

	/* Sleep until there is data to read */
	if ((status = wait_event_interruptible(encoder_wqueue, encoder_delta != 0)))
		return status;

    /**@todo:   find a solution for cases, where playground crashes,
                user turn on rotary like crazy, playground restarts and e.g.
                volume jumps to max. */

	if (copy_to_user(buf, &encoder_delta, 1) != 0)
		goto out;

	encoder_delta = 0;
	return 1;
out:

	return -EFAULT;
}

static s32 encoder_fops_open(struct inode *inode, struct file *filp)
{
	s32 status = 0;
	nonseekable_open(inode, filp);
	return status;
}

static s32 encoder_fops_release(struct inode *inode, struct file *filp)
{
	s32 status = 0;
	return status;
}

static unsigned int encoder_fops_poll(struct file *filp, poll_table *wait)
{

	unsigned int mask = 0;
	poll_wait(filp, &encoder_wqueue, wait);

	/* If there is data in buffer, reading is allowed
	 * Writing to buttons never makes sense, so dissallow */

	if (encoder_delta != 0)
		mask |= POLLIN | POLLRDNORM;

	return mask;
}

static const struct file_operations encoder_fops = {
		.owner = 	THIS_MODULE,
		.write = 	encoder_fops_write,
		.read =		encoder_fops_read,
		.open =		encoder_fops_open,
		.release = 	encoder_fops_release,
		.llseek = 	no_llseek,
		.poll =		encoder_fops_poll,
};

static struct class *encoder_class;

s32 espi_driver_encoder_setup(struct espi_driver *sb)
{
	s32 ret;

	encoder_delta = 0;

	ret = register_chrdev(ESPI_ENCODER_DEV_MAJOR, "spi", &encoder_fops);
	if (ret < 0)
		pr_err("%s: problem at register_chrdev\n", __func__);

	encoder_class = class_create(THIS_MODULE, "espi-encoder");
	if(IS_ERR(encoder_class))
		pr_err("%s: unable to create class\n", __func__);

	device_create(encoder_class, sb->dev, MKDEV(ESPI_ENCODER_DEV_MAJOR, 0), sb, "espi_encoder");

	return 0;
}

s32 espi_driver_encoder_cleanup(struct espi_driver *sb)
{
	device_destroy(encoder_class, MKDEV(ESPI_ENCODER_DEV_MAJOR, 0));
	class_destroy(encoder_class);
	unregister_chrdev(ESPI_ENCODER_DEV_MAJOR, "spi");

	return 0;
}

void espi_driver_encoder_poll(struct espi_driver *p)
{
	struct spi_transfer xfer;
	u8 rx_buff[3];
	u8 tx_buff[3];
	u8 tmp;
	extern int sck_hz;

	tx_buff[0] = 0xAA;

#if 0
	xfer.tx_buf = tx_buff;
	xfer.rx_buf = rx_buff;
	xfer.len = 3;
	xfer.bits_per_word = 8;
	xfer.delay_usecs = 0;
	xfer.speed_hz = sck_hz;

	espi_driver_scs_select((struct espi_driver*)p, ESPI_EDIT_PANEL_PORT, ESPI_EDIT_ENCODER_DEVICE);
	espi_driver_transfer(((struct espi_driver*)p)->spidev, &xfer);
	espi_driver_scs_select((struct espi_driver*)p, ESPI_EDIT_PANEL_PORT, 0);

	tmp = rx_buff[0] & rx_buff[1] & rx_buff[2];
	if(tmp == 0xFF)
		return;
#else
	xfer.tx_buf = tx_buff;
	xfer.rx_buf = rx_buff;
	xfer.len = 1;
	xfer.bits_per_word = 8;
	xfer.delay_usecs = 0;
	xfer.speed_hz = sck_hz;

	espi_driver_scs_select((struct espi_driver*)p, ESPI_EDIT_PANEL_PORT, ESPI_EDIT_ENCODER_DEVICE);
	espi_driver_transfer(((struct espi_driver*)p)->spidev, &xfer);
	espi_driver_scs_select((struct espi_driver*)p, ESPI_EDIT_PANEL_PORT, 0);
	tmp = rx_buff[0];

	tx_buff[0] = 0;
	espi_driver_scs_select((struct espi_driver*)p, ESPI_EDIT_PANEL_PORT, ESPI_EDIT_ENCODER_DEVICE);
	espi_driver_transfer(((struct espi_driver*)p)->spidev, &xfer);
	espi_driver_scs_select((struct espi_driver*)p, ESPI_EDIT_PANEL_PORT, 0);
	tmp &= rx_buff[0];
	if(tmp == 0xFF)
		return;
#endif

	//if(rx_buff[2] != 0 ) {
	if(rx_buff[0] != 0 ) {
		if (!(encoder_delta + (s8)rx_buff[0] > 127) &&
		    !(encoder_delta + (s8)rx_buff[0] < -128)) {
			encoder_delta += (s8) rx_buff[0];

			//printk("encoder delta: %d\n", (s8)encoder_delta);
			wake_up_interruptible(&encoder_wqueue);
		}
	}


#if 1
	if(tmp_boled_reset) {
		lpc8xx_boled_reset(p);
		tmp_boled_reset = 0;
	}
	if(tmp_boled_12v) {
		lpc8xx_boled_12v(p);
		tmp_boled_12v = 0;
	}
#endif
}

void lpc8xx_boled_reset(struct espi_driver *p)
{
	struct spi_transfer xfer;
	u8 rx_buff[3];
	u8 tx_buff[3];
	extern int sck_hz;

	tx_buff[0] = 0x77;

	xfer.tx_buf = tx_buff;
	xfer.rx_buf = rx_buff;
	xfer.len = 3;
	xfer.bits_per_word = 8;
	xfer.delay_usecs = 0;
	xfer.speed_hz = sck_hz;

	espi_driver_scs_select((struct espi_driver*)p, ESPI_EDIT_PANEL_PORT, ESPI_EDIT_ENCODER_DEVICE);
	espi_driver_transfer(((struct espi_driver*)p)->spidev, &xfer);
	espi_driver_scs_select((struct espi_driver*)p, ESPI_EDIT_PANEL_PORT, 0);

}

void lpc8xx_boled_12v(struct espi_driver *p)
{
	struct spi_transfer xfer;
	u8 rx_buff[3];
	u8 tx_buff[3];
	extern int sck_hz;

	tx_buff[0] = 0x44;

	xfer.tx_buf = tx_buff;
	xfer.rx_buf = rx_buff;
	xfer.len = 3;
	xfer.bits_per_word = 8;
	xfer.delay_usecs = 0;
	xfer.speed_hz = sck_hz;

	espi_driver_scs_select((struct espi_driver*)p, ESPI_EDIT_PANEL_PORT, ESPI_EDIT_ENCODER_DEVICE);
	espi_driver_transfer(((struct espi_driver*)p)->spidev, &xfer);
	espi_driver_scs_select((struct espi_driver*)p, ESPI_EDIT_PANEL_PORT, 0);

}


