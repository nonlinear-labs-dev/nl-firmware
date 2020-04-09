#include <linux/of_gpio.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>
#include "espi_driver.h"

#define ESPI_LED_DEV_MAJOR		301
#define LED_STATES_SIZE			12
static u8 *led_st;
static u8 *led_new_st;
static u64 lastUpdate = 0;
static DEFINE_MUTEX(led_state_lock);

static ssize_t led_fops_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	u32 i;
	u8 val, led_id;
	u8 tmp[count];

	if (copy_from_user(tmp, buf, count))
		return -EFAULT;

	//printk("Led.. %02X", tmp);

	mutex_lock(&led_state_lock);
	for(i=0; i<count; i++) {
		val = tmp[i] >> 7;
		led_id = tmp[i] & 0x7F;

		if(val == 0)
			led_new_st[LED_STATES_SIZE - 1 - led_id / 8] &= ~(1<<(led_id%8));
		else if(val == 1)
			led_new_st[LED_STATES_SIZE - 1 - led_id / 8] |= 1<<(led_id%8);
	}
	mutex_unlock(&led_state_lock);

	return count;
}

static const struct file_operations led_fops = {
		.owner = 	THIS_MODULE,
		.write = 	led_fops_write,
		.open =		nonseekable_open,
		.llseek = 	no_llseek,
};

static struct class *led_class;

s32 espi_driver_leds_setup(struct espi_driver *sb)
{
	s32 ret;

	led_st = kcalloc(LED_STATES_SIZE, sizeof(u8), GFP_KERNEL);
	if (!led_st)
		return -ENOMEM;

	led_new_st = kcalloc(LED_STATES_SIZE, sizeof(u8), GFP_KERNEL);
	if (!led_new_st)
		return -ENOMEM;

	ret = register_chrdev(ESPI_LED_DEV_MAJOR, "spi", &led_fops);
	if (ret < 0)
		pr_err("%s: problem at register_chrdev\n", __func__);

	led_class = class_create(THIS_MODULE, "espi-led");
	if(IS_ERR(led_class))
		pr_err("%s: unable to create class\n", __func__);

	device_create(led_class, sb->dev, MKDEV(ESPI_LED_DEV_MAJOR, 0), sb, "espi_led");

	return 0;
}

s32 espi_driver_leds_cleanup(struct espi_driver *sb)
{
	kfree(led_st);
	kfree(led_new_st);

	device_destroy(led_class, MKDEV(ESPI_LED_DEV_MAJOR, 0));
	class_destroy(led_class);
	unregister_chrdev(ESPI_LED_DEV_MAJOR, "spi");

	return 0;
}

void espi_driver_leds_poll(struct espi_driver *p)
{
	struct spi_transfer xfer;
	u32 i, update = 0;
	u64 now = get_jiffies_64();
	u64 diff = now - lastUpdate;
	unsigned int diffMS = jiffies_to_msecs(diff);
	unsigned int interval = 250;

	extern int sck_hz;

	mutex_lock(&led_state_lock);
	for(i=0; i<LED_STATES_SIZE; i++) {
		if(led_st[i] ^ led_new_st[i]) {
			led_st[i] = led_new_st[i];
			update = 1;
		}
	}
	mutex_unlock(&led_state_lock);

	update |= diffMS >= interval;

	if(update == 0)
		return;

	lastUpdate = now;

	xfer.tx_buf = led_st;
	xfer.rx_buf = NULL;
	xfer.len = LED_STATES_SIZE;
	xfer.bits_per_word = 8;
	xfer.delay_usecs = 0;
	xfer.speed_hz = sck_hz;

	espi_driver_scs_select((struct espi_driver*)p, ESPI_SELECTION_PANEL_PORT, ESPI_SELECTION_LEDS_DEVICE);
	espi_driver_transfer(((struct espi_driver*)p)->spidev, &xfer);
	gpio_set_value(((struct espi_driver *)p)->gpio_sap, 0);
	gpio_set_value(((struct espi_driver *)p)->gpio_sap, 1);
	espi_driver_scs_select((struct espi_driver*)p, ESPI_SELECTION_PANEL_PORT, 0);
}


