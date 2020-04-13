#include <linux/of_gpio.h>
#include <linux/poll.h>
#include <asm/uaccess.h>
#include "espi_driver.h"

#define ESPI_EPC_CTRL_DEV_MAJOR		320

static u8 epc_ctrl, epc_new_ctrl, epc_stat;

static ssize_t epc_ctrl_fops_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	u8 tmp[count];

	if (copy_from_user(tmp, buf, count))
		return -EINVAL;

	epc_new_ctrl = tmp[count-1];

	return count;
}

static ssize_t epc_ctrl_fops_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t status = 0;
	if(count < 1)
		return -EAGAIN;
	buf[0] = epc_stat;
	status = 1;
	return status;
}

static s32 epc_ctrl_fops_release(struct inode *inode, struct file *filp)
{
	s32 status = 0;
	return status;
}

static const struct file_operations epc_ctrl_fops = {
		.owner = 	THIS_MODULE,
		.write = 	epc_ctrl_fops_write,
		.read =		epc_ctrl_fops_read,
		.open =		nonseekable_open,
		.release = 	epc_ctrl_fops_release,
		.llseek = 	no_llseek,
};

static struct class *epc_ctrl_class;

s32 espi_driver_epc_ctrl_setup(struct espi_driver *sb)
{
	s32 ret;

	/* Start condition has to be like this, in order to control epc properly */
	epc_ctrl = 1;
	epc_new_ctrl = 0;

	epc_stat = 0;

	ret = register_chrdev(ESPI_EPC_CTRL_DEV_MAJOR, "spi", &epc_ctrl_fops);
	if (ret < 0)
		pr_err("%s: problem at register_chrdev\n", __func__);

	epc_ctrl_class = class_create(THIS_MODULE, "espi-epc-ctrl-stat");
	if(IS_ERR(epc_ctrl_class))
		pr_err("%s: unable to create class\n", __func__);

	device_create(epc_ctrl_class, sb->dev, MKDEV(ESPI_EPC_CTRL_DEV_MAJOR, 0), sb, "espi_epc_ctrl_stat");

	return 0;
}

s32 espi_driver_epc_ctrl_cleanup(struct espi_driver *sb)
{
	device_destroy(epc_ctrl_class, MKDEV(ESPI_EPC_CTRL_DEV_MAJOR, 0));
	class_destroy(epc_ctrl_class);
	unregister_chrdev(ESPI_EPC_CTRL_DEV_MAJOR, "spi");

	return 0;
}

void espi_driver_epc_control_poll(struct espi_driver *p)
{
	struct spi_transfer xfer;
	u8 update = 0;

	extern int sck_hz;

	if(epc_ctrl != epc_new_ctrl)
		update = 1;

	if(update == 0)
		return;

	epc_ctrl = epc_new_ctrl;
	xfer.tx_buf = &epc_ctrl;
	xfer.rx_buf = NULL;
	xfer.len = 1;
	xfer.bits_per_word = 8;
	xfer.delay_usecs = 0;
	xfer.speed_hz = sck_hz;

	espi_driver_scs_select((struct espi_driver*)p, ESPI_EPC_CTRL_STATE_PORT, ESPI_EPC_CONTROL_DEVICE);
	espi_driver_transfer(((struct espi_driver*)p)->spidev, &xfer);
	gpio_set_value(((struct espi_driver *)p)->gpio_sap, 0);
	gpio_set_value(((struct espi_driver *)p)->gpio_sap, 1);
	espi_driver_scs_select((struct espi_driver*)p, ESPI_EPC_CTRL_STATE_PORT, 0);
}

void espi_driver_epc_status_poll(struct espi_driver *p)
{
	struct spi_transfer xfer;
	u8 rxbuff[1];

	extern int sck_hz;

	xfer.tx_buf = NULL;
	xfer.rx_buf = rxbuff;
	xfer.len = 1;
	xfer.bits_per_word = 8;
	xfer.delay_usecs = 0;
	xfer.speed_hz = sck_hz;

	espi_driver_set_mode(((struct espi_driver*)p)->spidev, SPI_MODE_3);

	espi_driver_scs_select((struct espi_driver*)p, ESPI_EPC_CTRL_STATE_PORT, ESPI_EPC_STATE_DEVICE);
	gpio_set_value(((struct espi_driver *)p)->gpio_sap, 0);
	gpio_set_value(((struct espi_driver *)p)->gpio_sap, 1);
	espi_driver_transfer(((struct espi_driver*)p)->spidev, &xfer);
	espi_driver_scs_select((struct espi_driver*)p, ESPI_EPC_CTRL_STATE_PORT, 0);

	epc_stat = rxbuff[0];

	espi_driver_set_mode(((struct espi_driver*)p)->spidev, SPI_MODE_0);
}

