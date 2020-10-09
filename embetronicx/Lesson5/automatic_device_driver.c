/*************************************************************************************
 * FILE:        automatic_device_driver.c
 * DETAILS:     Simple linux driver (Automatic Creating a Device File)
 * AUTHOR:      Kunix
 * COMPILE:  	make
 * CLEAN: 		make clean
 * INSTALL: 	sudo insmod <driver_name>.ko
 * CHECK:		lsmod | grep <driver_name>
 * VERIFY:		dmesg
 * REMOVE:		sudo rmmod <driver_name> | <driver_name>.ko
 * INFO: 		modinfo <driver_name>.ko
 * CHECK:       ls -l /dev/ | grep "kunix_device"
  *************************************************************************************/
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/device.h>

dev_t dev = 0;
static struct class *dev_class;

/*
** Module init function
*/
static int __init automatic_device_init(void)
{
    /*Allocating Major number*/
    if((alloc_chrdev_region(&dev, 0, 1, "kunix_dev")) < 0)
    {
        printk(KERN_INFO "Cannot allocate major number for device\n");
        return -1;
    }
    printk(KERN_INFO "Major= %d Minor = %d \n", MAJOR(dev), MINOR(dev));

    /*Creating struct class*/
    if((dev_class = class_create(THIS_MODULE, "kunix_class")) == NULL)
    {
        printk(KERN_INFO "Cannot create the struct class for device\n");
        goto r_class;
    }

    /*Creating device*/
    if((device_create(dev_class, NULL, dev, NULL, "kunix_device")) == NULL)
    {
        printk(KERN_INFO "Cannot create the Device\n");
        goto r_device;
    }

    printk(KERN_INFO "Kernel Module Inserted Successfully...\n");
    return 0;

r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev,1);
    return -1;
}

/*
** Module exit function
*/
static void __exit automatic_device_exit(void)
{
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Kernel Module Removed Successfully...\n");
}

module_init(automatic_device_init);
module_exit(automatic_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kunix <kunix@gmail.com>");
MODULE_DESCRIPTION("Simple Linux Driver (Automatically Creating a Device file)");
MODULE_VERSION("1.2");