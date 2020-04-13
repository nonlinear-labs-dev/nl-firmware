/* 
   ESPI Driver for Ribbon Strip LEDS

LED ID layout (decimal)
     Left                                           Center                                           Right
Led # 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32
Upper 01 03 05 07 09 11 13 15 17 19 21 23 25 27 29 31 33 35 37 39 41 43 45 47 49 51 53 55 57 59 61 63 65
Lower 00 02 04 06 08 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50 52 54 56 58 60 62 64

LED intensity value (binary, only Bits 0..1 of value byte are used)
 00 : off
 01 : dim
 10 : normal
 11 : bright
 
Setting up a LED requires 2 bytes written to the driver:
 - LED ID
 - Intensity
 When writing an odd number of bytes, the last byte will be ignored to avoid going out
 of sync. But when sync is lost for other reasons, no attempt is made to resync !
 
 */

#include <linux/of_gpio.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>
#include "espi_driver.h"

#define ESPI_RIBBON_LED_DEV_MAJOR 303
#define ESD_INTERVAL              (250)  // force update of LEDs every 250ms, to cater for ESD etc glitches
#define RIBBON_LED_STATES_SIZE    (17)   // 66 LEDs * 2 bits for states each = 17 Bytes needed
static u8 *rb_led_st;
static u8 *rb_led_new_st;
static u64 lastUpdate = 0;
static DEFINE_MUTEX(rbled_state_lock);

// ---- write to driver function ----
// writes requested LED states to "new" buffer
static ssize_t rbled_write(struct file *filp, const char __user *buf_user, size_t count, loff_t *f_pos)
{
  char            buf[count];
  ssize_t         status = 0;
  u32             i;
  u8              val, led_id;
  static const u8 rot[] = { 0, 2, 1, 3 };  // mapping to the bit weighting the hardware requires

  if (copy_from_user(buf, buf_user, count))
    return -EFAULT;

  mutex_lock(&rbled_state_lock);  // keep espi_driver_rb_leds_poll() from interfering
  for (i = 0; (i + 1) < count; i += 2)
  {
    led_id = buf[i];
    val    = rot[buf[i + 1] & 0x3];  // get mapped bit weighting

    rb_led_new_st[(RIBBON_LED_STATES_SIZE - 1) - led_id / 4] &= ~(0x3 << ((led_id % 4) * 2));  // clear the 2 bits for the selected LED
    rb_led_new_st[(RIBBON_LED_STATES_SIZE - 1) - led_id / 4] |= val << ((led_id % 4) * 2);     // add in state bits
  }
  mutex_unlock(&rbled_state_lock);

  status = count;
  return status;
}

// file operations structure
static const struct file_operations rbled_fops = {
  .owner  = THIS_MODULE,
  .write  = rbled_write,
  .open   = nonseekable_open,
  .llseek = no_llseek,
};

static struct class *rbled_class;

// ---- driver setup (init) function ----
s32 espi_driver_rb_leds_setup(struct espi_driver *sb)
{
  s32 i, ret;

  // current and new LED states bit fields
  rb_led_st = kcalloc(RIBBON_LED_STATES_SIZE, sizeof(u8), GFP_KERNEL);
  if (!rb_led_st)
    return -ENOMEM;
  rb_led_new_st = kcalloc(RIBBON_LED_STATES_SIZE, sizeof(u8), GFP_KERNEL);
  if (!rb_led_new_st)
    return -ENOMEM;

  for (i = 0; i < RIBBON_LED_STATES_SIZE; i++)
  {
    rb_led_st[i]     = 0x00;  // initial current state is all OFF
    rb_led_new_st[i] = 0xFF;  // force all LEDs ON initially
  }

  ret = register_chrdev(ESPI_RIBBON_LED_DEV_MAJOR, "spi", &rbled_fops);
  if (ret < 0)
    pr_err("%s: problem at register_chrdev\n", __func__);

  rbled_class = class_create(THIS_MODULE, "ribbon-led");
  if (IS_ERR(rbled_class))
    pr_err("%s: unable to create class\n", __func__);

  device_create(rbled_class, sb->dev, MKDEV(ESPI_RIBBON_LED_DEV_MAJOR, 0), sb, "ribbon_led");

  return 0;
}

// ---- driver cleanup (de-init) function ----
s32 espi_driver_rb_leds_cleanup(struct espi_driver *sb)
{
  kfree(rb_led_st);
  kfree(rb_led_new_st);

  device_destroy(rbled_class, MKDEV(ESPI_RIBBON_LED_DEV_MAJOR, 0));
  class_destroy(rbled_class);
  unregister_chrdev(ESPI_RIBBON_LED_DEV_MAJOR, "spi");

  return 0;
}

// ---- send any newly changed states down to the LEDs ----
void espi_driver_rb_leds_poll(struct espi_driver *p)
{
  struct spi_transfer xfer;
  u32                 i, update = 0;

  u64          now    = get_jiffies_64();
  u64          diff   = now - lastUpdate;
  unsigned int diffMS = jiffies_to_msecs(diff);

  extern int sck_hz;

  mutex_lock(&rbled_state_lock);  // keep rbled_write() from interfering
  for (i = 0; i < RIBBON_LED_STATES_SIZE; i++)
  {
    if (rb_led_st[i] ^ rb_led_new_st[i])
    {
      rb_led_st[i] = rb_led_new_st[i];
      update       = 1;
    }
  }
  mutex_unlock(&rbled_state_lock);

  update |= (diffMS >= ESD_INTERVAL);  // periodic update due ("anti-ESD" update) ?

  if (update == 0)
    return;

  lastUpdate = now;

  xfer.tx_buf        = rb_led_st;
  xfer.rx_buf        = NULL;
  xfer.len           = RIBBON_LED_STATES_SIZE;
  xfer.bits_per_word = 8;
  xfer.delay_usecs   = 0;
  xfer.speed_hz      = sck_hz;

  espi_driver_scs_select(p, ESPI_RIBBON_LEDS_PORT, p->ribbon_leds_device);
  espi_driver_transfer(p->spidev, &xfer);
  gpio_set_value(p->gpio_sap, 0);
  gpio_set_value(p->gpio_sap, 1);
  espi_driver_scs_select(p, ESPI_RIBBON_LEDS_PORT, 0);
}

// EOF
