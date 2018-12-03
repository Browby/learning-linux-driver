#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/kdev_t.h>

int valueETX, arr_valueETX[4];
char *nameETX;
int cb_valueETX = 0;

//dev_t dev = MKDEV(235, 0);
dev_t dev = 0;
static struct class *dev_class;

module_param(valueETX, int, S_IRUSR|S_IWUSR);
module_param(nameETX, charp, S_IRUSR|S_IWUSR);
module_param_array(arr_valueETX, int, NULL, S_IRUSR|S_IWUSR);

int notify_param(const char *val, const struct kernel_param *kp)
{
  int res = param_set_int(val, kp);
  if(res==0){
    printk(KERN_INFO "Call back function called...\n");
    printk(KERN_INFO "New value of cb_valueETX = %d\n", cb_valueETX);
    return 0;
  }
  return -1;
}

const struct kernel_param_ops my_param_ops =
{
  .set = &notify_param,
  .get = &param_get_int,
};

module_param_cb(cb_valueETX, &my_param_ops, &cb_valueETX, S_IRUGO|S_IWUSR);

static int __init hello_world_init(void)
{
  int i;
  if ((alloc_chrdev_region(&dev, 0 ,1, "hello_world"))<0){
    printk(KERN_INFO "Cannot allocate major number for device\n");
    return -1;
  }
  printk(KERN_INFO "Major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));

  /* Creating struct class */
  if((dev_class = class_create(THIS_MODULE, "hello_class")) == NULL){
    printk(KERN_INFO "Cannot create the struct class for device\n");
    goto r_class;
  }

  /* Creating device */
  if((device_create(dev_class, NULL, dev, NULL, "hello_device")) == NULL){
    printk(KERN_INFO "Cannot create the Device\n");
    goto r_device;
  }

  //alloc_chrdev_region(&dev, 0, 1, "helloWorldModule_dev");
  //register_chrdev_region(dev, 1, "helloWorldModule_dev");
  printk(KERN_INFO "ValueETX = %d \n", valueETX);
  printk(KERN_INFO "cb_valueETX = %d \n", cb_valueETX);
  printk(KERN_INFO "NameETX = %s \n", nameETX);
  for (i = 0; i < (sizeof arr_valueETX / sizeof (int)); i++) {
    printk(KERN_INFO "Arr_value[%d] = %d\n", i, arr_valueETX[i]);
  }
  printk(KERN_INFO "Kernel module inserted successfully\n");
  return 0;

r_device:
  class_destroy(dev_class);
r_class:
  unregister_chrdev_region(dev,1);
  return -1;
}

void __exit hello_world_exit(void)
{
  device_destroy(dev_class, dev);
  class_destroy(dev_class);
  unregister_chrdev_region(dev, 1);
  printk(KERN_INFO "Kernel module removed successfully\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bruno Hendrickx");
MODULE_DESCRIPTION("Hello world");
MODULE_VERSION("0:0.1");
