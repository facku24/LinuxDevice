/*************************************************************************************
 * FILE:        driver.c
 * DETAILS:     Simple linux driver (statically allocating the Major and Minor number)
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
#include<linux/fs.h>

// creating the dev with our custom major and minor number
dev_t dev = MKDEV(235, 0);

/*
** Module Init function
*/
static int __init allocation_init(void)
{
    int ret = register_chrdev_region(dev, 1, "Kunix_Dev");
    if (ret == 0){
        printk(KERN_INFO "Major = %d Minor = %d\n", MAJOR(dev), MINOR(dev));
    }
    printk(KERN_INFO "Kernel Module Inserted Successfully...\n");
    return 0;
}

/*
** Module exit function
*/
static void __exit allocation_exit(void)
{
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Kernel Module Removed Successfully...\n");
}

module_init(allocation_init);
module_exit(allocation_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kunix <kunix@gmail.com>");
MODULE_DESCRIPTION("Smple Linux Driver (statically allocating the Major and Minor number)");
MODULE_VERSION("1.0");