/*************************************************************************************
 * FILE:        dynamic_allocation_module.c
 * DETAILS:     Simple linux driver (dynamic allocating the Major and Minor number)
 * AUTHOR:      Kunix
 * COMPILE:  	make
 * CLEAN: 		make clean
 * INSTALL: 	sudo insmod <driver_name>.ko
 * CHECK:		lsmod | grep <driver_name>
 * VERIFY:		dmesg
 * REMOVE:		sudo rmmod <driver_name> | <driver_name>.ko
 * INFO: 		modinfo <driver_name>.ko
 *************************************************************************************/
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>

dev_t dev = 0;

/*
** Module Init function
*/
static int __init dynamic_allocation_init(void)
{
    /*Allocation Major Number*/
    if((alloc_chrdev_region(&dev, 0, 1, "Kunix_Dev")) < 0)
    {
        printk(KERN_INFO "Cannot allocate major number for device 1\n");
        return -1;
    }
    printk(KERN_INFO "Major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));
    printk(KERN_INFO "Kernel Module Inserted Successfully...\n");

    return 0;
}

/*
** Module Exit Function
*/
static void __exit dynamic_allocation_exit(void)
{
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Kernel Module Removed Successfully...\n");
}

module_init(dynamic_allocation_init);
module_exit(dynamic_allocation_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kunix <kunix@gmail.com>");
MODULE_DESCRIPTION("Simple Linux Driver (Dynamic allocation the Major and Minor Number)");
MODULE_VERSION("1.1");
