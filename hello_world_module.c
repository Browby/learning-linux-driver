#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

static int __init hello_world_init(void)
{
  printk(KERN_INFO "This is a simple module\n");
  printk(KERN_INFO "Kernel module inserted successfully\n");
  return 0;
}

void __exit hello_world_exit(void)
{
  printk(KERN_INFO "Kernel module removed successfully\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bruno Hendrickx");
MODULE_DESCRIPTION("Hello world");
MODULE_VERSION("0:0.1");
