#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;

static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t etx_write(struct file *filp, const char *buf, size_t len, loff_t *off);

static struct file_operations fops =
{
.owner      = THIS_MODULE,
.read       = etx_read,
.write      = etx_write,
.open       = etx_open,
.release    = etx_release,
};

static int etx_open(struct inode *inode, struct file *file)
{
  printk(KERN_INFO "Driver Open function called... !!!\n");
  return 0;
}

static int etx_release(struct inode *inode, struct file *file)
{
  printk(KERN_INFO "Driver Release function called...!!!\n");
  return 0;
}

static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
  printk(KERN_INFO "Driver read function called ...!!!\n");
  return 0;
}

static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
  printk(KERN_INFO "Driver write function called ...!!!!\n");
  return len;
}

static int __init etx_driver_init(void)
{
  /* Allocating Major number */
  if ((alloc_chrdev_region(&dev, 0 ,1, "ext_dev"))<0){
    printk(KERN_INFO "Cannot allocate major number for device\n");
    return -1;
  }
  printk(KERN_INFO "Major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));

  /* Creating cdev structure*/
  cdev_init(&etx_cdev, &fops);

  /*Adding character device to the system*/
  if((cdev_add(&etx_cdev, dev, 1)) < 0){
    printk(KERN_INFO "Cannot add the device to the system\n");
    goto r_class;
  }

  /* Creating struct class */
  if((dev_class = class_create(THIS_MODULE, "etc_class")) == NULL){
    printk(KERN_INFO "Cannot create the struct class for device\n");
    goto r_class;
  }

  /* Creating device */
  if((device_create(dev_class, NULL, dev, NULL, "etx_device")) == NULL){
    printk(KERN_INFO "Cannot create the Device\n");
    goto r_device;
  }

  printk(KERN_INFO "Kernel module inserted successfully\n");
  return 0;

r_device:
  class_destroy(dev_class);
r_class:
  unregister_chrdev_region(dev,1);
  return -1;
}

static void __exit etx_driver_exit(void)
{
  device_destroy(dev_class, dev);
  class_destroy(dev_class);
  cdev_del(&etx_cdev);
  unregister_chrdev_region(dev, 1);
  printk(KERN_INFO "Kernel module removed successfully\n");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bruno Hendrickx");
MODULE_DESCRIPTION("A simple device driver");
MODULE_VERSION("0:0.1");
