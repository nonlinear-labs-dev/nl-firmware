/*
   ESPI Driver for PanelUnit LEDS
   
LED ID layout (hex)

  Select.Panel #1 |  Select.Panel #2  |     Edit Panel    |  Select.Panel #3  |  Select.Panel #4
00 04 08 0C 10 14 | 18 1C 20 24 28 2C |                   | 30 34 38 3C 40 44 | 48 4C 50 54 58 5C
01 05 09 0D 11 15 | 19 1D 21 25 29 2D |                   | 31 35 39 3D 41 45 | 49 4D 51 55 59 5D
02 06 0A 0E 12 16 | 1A 1E 22 26 2A 2E |                   | 32 36 3A 3E 42 46 | 4A 4E 52 56 5A 5E
03 07 0B 0F 13 17 | 1B 1F 23 27 2B 2F |                   | 33 37 3B 3F 43 47 | 4B 4F 53 57 5B 5F


Setting up a LED requires 1 byte written to the driver:
LED ID        (bit 7 cleared) ==> LED off
LED ID | 0x80 (bit 7 set)     ==> LED on

The LEDs are force-updated in specific time intervals to care for ESD-induced upsets,
and to show correct display when the Panel Unit is plugged in live.

*/

#include <linux/of_gpio.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>
#include "espi_driver.h"

#define ESD_INTERVAL             (250)  // force update of LEDs every 250ms, to cater for ESD etc glitches
#define NUMBER_OF_LEDS_PER_PANEL (24)
#define NUMBER_OF_PANELS         (4)
#define NUMBER_OF_LEDS           (NUMBER_OF_LEDS_PER_PANEL * NUMBER_OF_PANELS)
#define LED_STATES_SIZE          ((u16)((NUMBER_OF_LEDS + 7) / 8))  // number of bytes to store the LED bits
static u8 *led_st;
static u8 *led_new_st;
static u64 lastUpdate = 0;
static DEFINE_MUTEX(led_state_lock);

// ---- write to driver function ----
// writes requested LED states to "new" buffer
static ssize_t led_fops_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
  u32 i;
  u8  led_id;
  u8  tmp[count];

  if (copy_from_user(tmp, buf, count))
    return -EFAULT;

  //printk("Led.. %02X", tmp);

  mutex_lock(&led_state_lock);  // keep espi_driver_leds_poll() from interfering
  for (i = 0; i < count; i++)
  {
    led_id = tmp[i] & 0x7F;
    if (tmp[i] & 0x80)  // "ON" bit set ?
      led_new_st[(LED_STATES_SIZE - 1) - led_id / 8] |= 1 << (led_id % 8);
    else
      led_new_st[(LED_STATES_SIZE - 1) - led_id / 8] &= ~(1 << (led_id % 8));
  }
  mutex_unlock(&led_state_lock);

  return count;
}

// file operations structure
static const struct file_operations led_fops = {
  .owner  = THIS_MODULE,
  .write  = led_fops_write,
  .open   = nonseekable_open,
  .llseek = no_llseek,
};

static struct class *led_class;

// ---- driver setup (init) function ----
s32 espi_driver_leds_setup(struct espi_driver *sb)
{
  s32 ret;
  int i;

  // current and new LED states bit fields
  led_st     = kcalloc(LED_STATES_SIZE, sizeof(u8), GFP_KERNEL);
  led_new_st = kcalloc(LED_STATES_SIZE, sizeof(u8), GFP_KERNEL);
  if (!led_st || !led_new_st)
    return -ENOMEM;

  for (i = 0; i < LED_STATES_SIZE; i++)
  {
    led_st[i]     = 0x00;  // initial current state is all OFF
    led_new_st[i] = 0xFF;  // force all LEDs ON initially
  }

  ret = register_chrdev(ESPI_LED_DEV_MAJOR, "spi", &led_fops);
  if (ret < 0)
    pr_err("%s: problem at register_chrdev\n", __func__);

  led_class = class_create(THIS_MODULE, "espi-led");
  if (IS_ERR(led_class))
    pr_err("%s: unable to create class\n", __func__);

  device_create(led_class, sb->dev, MKDEV(ESPI_LED_DEV_MAJOR, 0), sb, "espi_led");

  return 0;
}

// ---- driver cleanup (de-init) function ----
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
  u32                 i, update = 0;

  u64          now    = get_jiffies_64();
  u64          diff   = now - lastUpdate;
  unsigned int diffMS = jiffies_to_msecs(diff);

  extern int sck_hz;

  mutex_lock(&led_state_lock);  // keep led_fops_write() from interfering
  for (i = 0; i < LED_STATES_SIZE; i++)
  {
    if (led_st[i] ^ led_new_st[i])
    {
      led_st[i] = led_new_st[i];
      update    = 1;
    }
  }
  mutex_unlock(&led_state_lock);

  update |= (diffMS >= ESD_INTERVAL);  // periodic update due ("anti-ESD" update) ?

  if (update == 0)
    return;

  lastUpdate = now;

  xfer.tx_buf        = led_st;
  xfer.rx_buf        = NULL;
  xfer.len           = LED_STATES_SIZE;
  xfer.bits_per_word = 8;
  xfer.delay_usecs   = 0;
  xfer.speed_hz      = sck_hz;

  espi_driver_scs_select(p, ESPI_SELECTION_PANEL_PORT, ESPI_SELECTION_LEDS_DEVICE);
  espi_driver_transfer(p->spidev, &xfer);
  gpio_set_value(p->gpio_sap, 0);
  gpio_set_value(p->gpio_sap, 1);
  espi_driver_scs_select(p, ESPI_SELECTION_PANEL_PORT, 0);
}

// EOF
