/******************************************************************************/
/** @file	spi_lpc_bb.c
    @date	last modified date/name: 2014-12-04 DTZ
    @brief    	This module takes care about the communication with the LPC via
    SPI.
    @author	Nemanja Nikodijevic [2014-03-02]
*******************************************************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>
#include <linux/init.h>
#include <linux/spi/spi.h>
#include <linux/fs.h>       // Write data to sysfs
#include <linux/of_gpio.h>  // nni: added for device tree
#include <linux/platform_device.h>
#include <linux/delay.h>

#include <linux/uaccess.h>

/* speed can only be f_max/(2^n) -> f_max = 24 MHz */
#define LPC_BB_DRIVER_MAX_SPEED    3000000
#define LPC_BB_DRIVER_XFER_SIZE    1024
#define LPC_BB_DRIVER_DEV_MAJOR    300
#define LPC_BB_DRIVER_RX_BUFF_SIZE 1024
#define LPC_BB_DRIVER_RX_BUFF_MASK (LPC_BB_DRIVER_RX_BUFF_SIZE - 1)

struct lpc_bb_driver
{
  struct delayed_work work;
  struct device *     dev;
  struct spi_device * spidev;

  int rdy_gpio;
  int prq_gpio;
  int lpc_hb_gpio;
  int lpc_spare_gpio;
  int cs_gpio;
};

static DEFINE_MUTEX(tx_buff_lock);
static struct semaphore rx_buff_empty
    = __SEMAPHORE_INITIALIZER(rx_buff_empty, LPC_BB_DRIVER_RX_BUFF_SIZE);         // SPI_LPC_BB_RX_BUFF_SIZE
static struct semaphore rx_buff_full = __SEMAPHORE_INITIALIZER(rx_buff_full, 0);  //0

static u8 *     tx_buff;  //[2][SPI_LPC_BB_XFER_SIZE];
static u8       tx_write_buffer;
static unsigned tx_write_buffer_offset;
static u8 *     rxb;      //[SPI_LPC_BB_XFER_SIZE];
static u8 *     rx_buff;  //[SPI_LPC_BB_RX_BUFF_SIZE];
static unsigned rx_bytes_to_read;

static volatile size_t rx_buff_head;
static volatile size_t rx_buff_tail;

static u8  no_hb_count;
static u8  no_rdy;
static u8  xfer_repeat;
static u8 *txb_repeat;

static struct workqueue_struct *workqueue;

// **************************************************************************
static int lpc_bb_driver_check_hb(int hb_gpio)
{
  int hb_val, hb_tmp;

  hb_val = gpio_get_value(hb_gpio);
  usleep_range(62, 65);
  hb_tmp = gpio_get_value(hb_gpio);

  if (hb_tmp != hb_val)
    return 1;

  usleep_range(63, 65);
  hb_tmp = gpio_get_value(hb_gpio);

  if (hb_tmp != hb_val)
    return 1;

  return 0;
}

static void lpc_bb_driver_package_parse(void)
{
  int i = 4;

  if ((rxb[0] != 0xFF) || (rxb[3] != 0xFF))
    return;

  rx_bytes_to_read = rxb[1] + (rxb[2] << 8);

  while (rx_bytes_to_read)
  {
    if (down_interruptible(&rx_buff_empty) == 0)
    {
      rx_buff[rx_buff_head & LPC_BB_DRIVER_RX_BUFF_MASK] = rxb[i++];
      rx_buff_head++;

      up(&rx_buff_full);
      rx_bytes_to_read--;
    }
    else
    {
      printk("missed a packet!\n");
      return;
    }
  }
}

static u8 *lpc_bb_driver_get_txb(void)
{
  u32 *msg_size;
  u8 * txb;

  /** request tx buffer */
  mutex_lock(&tx_buff_lock);
  txb                    = tx_buff + tx_write_buffer * LPC_BB_DRIVER_XFER_SIZE;  //[tx_write_buffer];
  tx_write_buffer        = (tx_write_buffer + 1) % 2;
  msg_size               = (u32 *) (tx_buff + tx_write_buffer * LPC_BB_DRIVER_XFER_SIZE);
  *msg_size              = 0xFF0000FF;
  tx_write_buffer_offset = 4;
  mutex_unlock(&tx_buff_lock);

  return txb;
}

// **************************************************************************
static int lpc_bb_driver_transfer(struct spi_device *dev, u8 *txb)
{
  struct spi_transfer xfer;
  struct spi_message  msg;
  int                 status;

  /** prepare and perform the transfer */
  xfer.tx_buf        = txb;
  xfer.rx_buf        = rxb;
  xfer.len           = LPC_BB_DRIVER_XFER_SIZE;
  xfer.speed_hz      = LPC_BB_DRIVER_MAX_SPEED;
  xfer.bits_per_word = 8;
  xfer.delay_usecs   = 0;
  xfer.tx_nbits = xfer.rx_nbits = SPI_NBITS_SINGLE;

  spi_message_init(&msg);
  spi_message_add_tail(&xfer, &msg);

  status = spi_sync(dev, &msg);

  return status;
}

// **************************************************************************
static void lpc_bb_driver_poll(struct delayed_work *p)
{
  struct lpc_bb_driver *spi = (struct lpc_bb_driver *) p;
  int                   rdy, prq, txw, hb;
  u8 *                  txb;

  //printk("Workqueue %i",  ((struct spilpcbb *)p)->counter++);
  rdy = gpio_get_value(spi->rdy_gpio);
  prq = gpio_get_value(spi->prq_gpio);
  hb  = lpc_bb_driver_check_hb(spi->lpc_hb_gpio);
  txw = tx_write_buffer_offset - 4;

  if (!hb)
  {
    if (no_hb_count < 5)
    {
      no_hb_count++;
    }
    goto exit;
  }

  if (no_hb_count)
    no_hb_count = 0;

  if (rdy && (prq || txw))
  {
    no_rdy = 0;

    if (xfer_repeat)
      txb = txb_repeat;
    else
      txb = lpc_bb_driver_get_txb();

    gpio_set_value(spi->cs_gpio, 0);
    lpc_bb_driver_transfer(((struct lpc_bb_driver *) p)->spidev, txb);
    gpio_set_value(spi->cs_gpio, 1);

    usleep_range(1500, 2000);

    hb = lpc_bb_driver_check_hb(spi->lpc_hb_gpio);
    if (hb)
    {
      xfer_repeat = 0;
      lpc_bb_driver_package_parse();
    }
    else
    {
      xfer_repeat = 1;
      txb_repeat  = txb;
    }
  }
  else if (!rdy)
    no_rdy = 1;

exit: /* printk("polling... rx_buff_head = %d\n",rx_buff_head); */
  queue_delayed_work(workqueue, p, msecs_to_jiffies(10));
}

static ssize_t lpc_bb_driver_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
  ssize_t status = 0;
  u32 *   msg_size;

  /* If the communication is dead, return */
  if (no_hb_count >= 5)
    return -EIO;

  /* If tx buffer is full, return */
  if ((tx_write_buffer_offset + count) > LPC_BB_DRIVER_XFER_SIZE)
  {
    if (no_rdy)
      return -EBUSY;
    return -EAGAIN;
  }

  mutex_lock(&tx_buff_lock);

  status = copy_from_user((tx_buff + tx_write_buffer * LPC_BB_DRIVER_XFER_SIZE + tx_write_buffer_offset), buf, count);
  if (status == 0)
  {
    tx_write_buffer_offset += count;
    msg_size = (u32 *) (tx_buff + tx_write_buffer * LPC_BB_DRIVER_XFER_SIZE);
    *msg_size += count << 8;
    status = count;
  }
  else
    status = -EFAULT;

  mutex_unlock(&tx_buff_lock);

  return status;
}

static ssize_t lpc_bb_driver_read(struct file *filp, char __user *buf_user, size_t count, loff_t *f_pos)
{
  unsigned i;
  char     buf[count];

  /* If in non-blocking mode and no data to read, return */
  if (filp->f_flags & O_NONBLOCK && rx_buff_head == rx_buff_tail)
    return -EAGAIN;

  for (i = 0; i < count; i++)
  {
    if (down_interruptible(&rx_buff_full))
    {
      printk("interrupted during read!\n");
      return i;
    }

    copy_to_user(buf_user + i, &rx_buff[rx_buff_tail & LPC_BB_DRIVER_RX_BUFF_MASK], 1);
    rx_buff_tail++;

    up(&rx_buff_empty);

    if (rx_buff_head == rx_buff_tail)
      return i + 1;
  }

  return count;
}

static int lpc_bb_driver_open(struct inode *inode, struct file *filp)
{
  int status = 0;

  nonseekable_open(inode, filp);

  return status;
}

static int lpc_bb_driver_release(struct inode *inode, struct file *filp)
{
  int status = 0;

  return status;
}

static const struct file_operations lpc_bb_driver_fops = {
  .owner   = THIS_MODULE,
  .write   = lpc_bb_driver_write,
  .read    = lpc_bb_driver_read,
  .open    = lpc_bb_driver_open,
  .release = lpc_bb_driver_release,
  .llseek  = no_llseek,
};

static struct class *lpc_bb_driver_class;

// **************************************************************************
static int lpc_bb_driver_probe(struct spi_device *dev)
{
  int                   ret = 0;
  struct lpc_bb_driver *sb;
  struct device_node *  dn = dev->dev.of_node;
  struct device *       tmpd;
  u32 *                 msg_size;

  printk("lpc_bb_driver_probe\n");

  sb = devm_kzalloc(&dev->dev, sizeof(struct lpc_bb_driver), GFP_KERNEL);
  if (!sb)
  {
    dev_err(&dev->dev, "%s: unable to kzalloc for spi-lpc-bb\n", __func__);
    return -ENOMEM;
  }

  rxb = kcalloc(LPC_BB_DRIVER_XFER_SIZE, sizeof(u8), GFP_KERNEL);
  if (!rxb)
  {
    dev_err(&dev->dev, "%s: unable to kcalloc for rxb\n", __func__);
    return -ENOMEM;
  }

  tx_buff = kcalloc(2 * LPC_BB_DRIVER_XFER_SIZE, sizeof(u8), GFP_KERNEL);
  if (!tx_buff)
  {
    dev_err(&dev->dev, "%s: unable to kcalloc for txb\n", __func__);
    return -ENOMEM;
  }

  rx_buff = kcalloc(LPC_BB_DRIVER_RX_BUFF_SIZE, sizeof(u8), GFP_KERNEL);
  if (!rx_buff)
  {
    dev_err(&dev->dev, "%s: unable to kcalloc for rx_buff\n", __func__);
    return -ENOMEM;
  }

  /** check DT entries and request gpios */
  sb->rdy_gpio = of_get_named_gpio(dn, "rdy-gpio", 0);
  if (!gpio_is_valid(sb->rdy_gpio))
  {
    dev_err(&dev->dev, "%s: rdy-gpio in the device tree incorrect\n", __func__);
    return -EINVAL;
  }
  sb->prq_gpio = of_get_named_gpio(dn, "prq-gpio", 0);
  if (!gpio_is_valid(sb->prq_gpio))
  {
    dev_err(&dev->dev, "%s: prq-gpio in the device tree incorrect\n", __func__);
    return -EINVAL;
  }
  sb->lpc_hb_gpio = of_get_named_gpio(dn, "lpc-hb-gpio", 0);
  if (!gpio_is_valid(sb->lpc_hb_gpio))
  {
    dev_err(&dev->dev, "%s: lpc-hb-gpio in the device tree incorrect\n", __func__);
    return -EINVAL;
  }
  sb->lpc_spare_gpio = of_get_named_gpio(dn, "lpc-spare-gpio", 0);
  if (!gpio_is_valid(sb->lpc_spare_gpio))
  {
    dev_err(&dev->dev, "%s: lpc-spare-gpio in the device tree incorrect\n", __func__);
    return -EINVAL;
  }
  sb->cs_gpio = of_get_named_gpio(dn, "lpc-cs-gpio", 0);
  if (!gpio_is_valid(sb->cs_gpio))
  {
    dev_err(&dev->dev, "%s: lpc-cs-gpio in the device tree incorrect\n", __func__);
    return -EINVAL;
  }
  ret = devm_gpio_request_one(&dev->dev, sb->rdy_gpio, GPIOF_IN, "rdy_gpio");
  ret = devm_gpio_request_one(&dev->dev, sb->prq_gpio, GPIOF_IN, "prq_gpio");
  ret = devm_gpio_request_one(&dev->dev, sb->lpc_hb_gpio, GPIOF_IN, "lpc_hb_gpio");
  ret = devm_gpio_request_one(&dev->dev, sb->lpc_spare_gpio, GPIOF_OUT_INIT_HIGH, "lpc_spare_gpio");
  ret = devm_gpio_request_one(&dev->dev, sb->cs_gpio, GPIOF_OUT_INIT_HIGH, "cs_gpio");

  /** prepare the spi controller */
  dev->mode          = SPI_MODE_1;
  dev->max_speed_hz  = LPC_BB_DRIVER_MAX_SPEED;
  dev->bits_per_word = 8;
  if (spi_setup(dev))
  {
    dev_err(&dev->dev, "%s: spi setup incorrect\n", __func__);
    return -EINVAL;
  }

  /** create the device */
  tmpd                   = device_create(lpc_bb_driver_class, &dev->dev, MKDEV(LPC_BB_DRIVER_DEV_MAJOR, 0), sb, "lpc_bb_driver");
  rx_bytes_to_read       = 0;
  tx_write_buffer        = 0;
  tx_write_buffer_offset = 4;
  msg_size               = (u32 *) tx_buff;
  *msg_size              = 0xFF0000FF;
  rx_buff_head           = 0;
  rx_buff_tail           = 0;
  no_hb_count            = 0;

  sb->spidev = dev;
  sb->dev    = &dev->dev;
  dev_set_drvdata(&dev->dev, (void *) sb);
  dev_info(&dev->dev, "spi registered, item=0x%p\n", (void *) sb);

  INIT_DELAYED_WORK(&(sb->work), (work_func_t) lpc_bb_driver_poll);
  queue_delayed_work(workqueue, &(sb->work), msecs_to_jiffies(10));  // jiffies / HZ -> ms, so 10ms

  return ret;
}

// **************************************************************************
static int lpc_bb_driver_remove(struct spi_device *spi)
{
  struct lpc_bb_driver *sb = (struct lpc_bb_driver *) dev_get_drvdata(&spi->dev);

  printk("spi_lpc_bb_remove\n");

  cancel_delayed_work(&(sb->work));

  device_destroy(lpc_bb_driver_class, MKDEV(LPC_BB_DRIVER_DEV_MAJOR, 0));
  class_destroy(lpc_bb_driver_class);
  unregister_chrdev(LPC_BB_DRIVER_DEV_MAJOR, "spi");

  kfree(rxb);
  kfree(rx_buff);
  kfree(tx_buff);

  return 0;
}

// **************************************************************************
static struct spi_driver lpc_bb_driver_driver = {
  .driver = {
      .name  = "lpc_bb_driver",
      .bus   = &spi_bus_type,
      .owner = THIS_MODULE,
  },
  .probe  = lpc_bb_driver_probe,
  .remove = lpc_bb_driver_remove,
};

// **************************************************************************
static int __init lpc_bb_driver_init(void)
{
  int ret;

#warning### SPI speed is set to a debug-able speed ###

  printk("lpc_bb_driver_init --\n");
  printk("lpc_bb_driver VERSION: 17-12-2014\n");

  ret = register_chrdev(LPC_BB_DRIVER_DEV_MAJOR, "spi", &lpc_bb_driver_fops);
  if (ret < 0)
    pr_err("%s: problem at register_chrdev\n", __func__);

  lpc_bb_driver_class = class_create(THIS_MODULE, "lpc_bb_driver");
  if (IS_ERR(lpc_bb_driver_class))
    pr_err("%s: unable to create class\n", __func__);

  workqueue = create_workqueue("lpc_bb_driver_queue");
  if (workqueue == NULL)
  {
    pr_err("%s: unable to create workqueue\n", __func__);
    return -1;
  }

  ret = spi_register_driver(&lpc_bb_driver_driver);
  if (ret)
    pr_err("%s: problem at spi_register_driver\n", __func__);

  no_rdy      = 0;
  no_hb_count = 0;
  xfer_repeat = 0;

  printk("registration done --\n");

  return ret;
}
module_init(lpc_bb_driver_init);

// **************************************************************************
static void __exit lpc_bb_driver_exit(void)
{
  printk("lpc_bb_driver_exit --\n");

  spi_unregister_driver(&lpc_bb_driver_driver);
}
module_exit(lpc_bb_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nemanja Nikodijevic");
MODULE_DESCRIPTION("lpc_bb_driver");
MODULE_VERSION("2016-03-02");
