/*************************************************************************************
 * FILE:        device_file_module.c
 * DETAILS:     Simple linux driver (Device File Module)
 * AUTHOR:      Kunix
 * COMPILE:  	make
 * CLEAN: 		make clean
 * INSTALL: 	sudo insmod <driver_name>.ko
 * CHECK:		lsmod | grep <driver_name>
 * VERIFY:		dmesg
 * REMOVE:		sudo rmmod <driver_name> | <driver_name>.ko
 * INFO: 		modinfo <driver_name>.ko
 * CREATE:      sudo mknod -m <permissions> <name> <device type> <major> <minor>
 * EXAMPLE:     sudo mknod -m 666 /dev/etx_device c 246 0
 *************************************************************************************/
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>

dev_t dev = 0;

static int __init device_file_init(void)
{
    /*Allocation Major Number*/
    if((alloc_chrdev_region(&dev, 0, 1, "kunix_dev")) < 0)
    {
        printk(KERN_INFO "Cannot allocate major number for device\n");
        return -1;
    }
    printk(KERN_INFO "Major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));
    printk(KERN_INFO "Kernel Module Inserted Successfully...\n");

    return 0;
}

void __exit device_file_exit(void)
{
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Kernel Module Removed Successfully...\n");
}

module_init(device_file_init);
module_exit(device_file_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kunix <kunix@gmail.com>");
MODULE_DESCRIPTION("A Simple Driver for Device File Creation");
MODULE_VERSION("1.1");
