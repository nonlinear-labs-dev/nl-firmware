#include <linux/poll.h>
#include <linux/of_gpio.h>
#include "espi_driver.h"

#define ESPI_MAIN_CTRL_DEV_MAJOR		321
static u8 lpc_ctrl;
static u8 update = 0;

/*******************************************************************************

  This module can bring the LPC into a ISP Programming state

  Shift Reg:
  QA/0 - BBB_SEL_LPCPROG_#KS
  QB/1 - LPC_DBG_RST_#CPU
  QC/2 - LPC_DBG_RST_#ISP

  BBB programmin LPC sequence:
  - set #ISP to low
  - set #RST low and high again
  - release #ISP to high again
  -> LPC is in programming mode now
  - programm via USB/UART
  - set #RST low and high again

*******************************************************************************/
static ssize_t lpc_ctrl_fops_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t status = 0;
	u8 tmp[count];

	if (copy_from_user(tmp, buf, count))
		return -EINVAL;

	lpc_ctrl = tmp[0];

	printk("New value for lpc_ctrl: %02X\n", tmp[0]);

	update = 1;
	status = count;
	return status;
}

static s32 lpc_ctrl_fops_release(struct inode *inode, struct file *filp)
{
	s32 status = 0;
	return status;
}

static const struct file_operations lpc_ctrl_fops = {
		.owner = 	THIS_MODULE,
		.write = 	lpc_ctrl_fops_write,
		.open =		nonseekable_open,
		.release = 	lpc_ctrl_fops_release,
		.llseek = 	no_llseek,
};

static struct class *lpc_ctrl_class;

s32 espi_driver_lpc_ctrl_setup(struct espi_driver *sb)
{
	s32 ret;

	lpc_ctrl = 0;

	ret = register_chrdev(ESPI_MAIN_CTRL_DEV_MAJOR, "spi", &lpc_ctrl_fops);
	if (ret < 0)
		pr_err("%s: problem at register_chrdev\n", __func__);

	lpc_ctrl_class = class_create(THIS_MODULE, "espi-main-ctrl");
	if(IS_ERR(lpc_ctrl_class))
		pr_err("%s: unable to create class\n", __func__);

	device_create(lpc_ctrl_class, sb->dev, MKDEV(ESPI_MAIN_CTRL_DEV_MAJOR, 0), sb, "espi_lpc_ctrl");

	return 0;
}

s32 espi_driver_lpc_ctrl_cleanup(struct espi_driver *sb)
{
	device_destroy(lpc_ctrl_class, MKDEV(ESPI_MAIN_CTRL_DEV_MAJOR, 0));
	class_destroy(lpc_ctrl_class);
	unregister_chrdev(ESPI_MAIN_CTRL_DEV_MAJOR, "spi");

	return 0;
}

void espi_driver_lpc_ctrl_poll(struct espi_driver *p)
{
	struct spi_transfer xfer;
	u8 txbuff[1];
	extern int sck_hz;

	if(update == 0)
		return;
	update = 0;

	txbuff[0] = lpc_ctrl;

	xfer.tx_buf = txbuff;
	xfer.rx_buf = NULL;
	xfer.len = 1;
	xfer.bits_per_word = 8;
	xfer.delay_usecs = 0;
	xfer.speed_hz = sck_hz;

	espi_driver_scs_select((struct espi_driver*)p, ESPI_MAIN_CONTROL_PORT, ESPI_MAIN_CONTROL_DEVICE);
	espi_driver_transfer(((struct espi_driver*)p)->spidev, &xfer);
	gpio_set_value(((struct espi_driver *)p)->gpio_sap, 0);
	gpio_set_value(((struct espi_driver *)p)->gpio_sap, 1);
	espi_driver_scs_select((struct espi_driver*)p, ESPI_MAIN_CONTROL_PORT, 0);
}


