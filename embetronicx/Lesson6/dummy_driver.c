/*************************************************************************************
 * FILE:        dummy_driver.c
 * DETAILS:     Simple linux driver (File Operations)
 * AUTHOR:      Kunix
 * COMPILE:  	make
 * CLEAN: 		make clean
 * INSTALL: 	sudo insmod <driver_name>.ko
 * CHECK:		lsmod | grep <driver_name>
 * VERIFY:		dmesg
 * REMOVE:		sudo rmmod <driver_name> | <driver_name>.ko
 * INFO: 		modinfo <driver_name>.ko
 * CHECK:       ls -l /dev/ | grep "kunix_device"
 * TEST:        sudo bash -c 'echo 1 > /dev/kunix_device'
  *************************************************************************************/
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>

dev_t dev = 0;
static struct class *dev_class;
static struct cdev kunix_cdev;

/*
** Function Prototypes
*/
static int          __init kunix_driver_init(void);
static void         __exit kunix_driver_exit(void);
static int          kunix_open(struct inode *inode, struct file *file);
static int          kunix_release(struct inode *inode, struct file *file);
static ssize_t      kunix_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t      kunix_write(struct file *filp, const char *buf, size_t len, loff_t *off);

static struct file_operations fops = 
{
    .owner      = THIS_MODULE,
    .read       = kunix_read,
    .write      = kunix_write,
    .open       = kunix_open,
    .release    = kunix_release,
};

/*
** This function will be called when we open the Device file
*/
static int kunix_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Driver Open Function Called...!!!\n");
    return 0;
}

/*
** This function will be called when we close the Device file
*/
static int kunix_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Driver Release Function Called...!!!\n");
    return 0;
}

/*
** This function will be called when we read the Device file
*/
static ssize_t kunix_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Driver Read Function Called...!!!\n");
    return 0;
}

/*
** This function will be called when we write the Device file
*/
static ssize_t kunix_write(struct file *filp, const char *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Driver Write Function Called...!!!\n");
    return len;
}

/*
** Module Init function
*/
static int __init kunix_driver_init(void)
{
    /*Allocating Major number*/
    if((alloc_chrdev_region(&dev, 0, 1, "kunix_dev")) < 0)
    {
        printk(KERN_INFO "Cannot allocate major number\n");
        return -1;
    }
    printk(KERN_INFO "Major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));

    /*Creating cdev structure*/
    cdev_init(&kunix_cdev, &fops);

    /*Adding character device to the system*/
    if((cdev_add(&kunix_cdev, dev, 1)) < 0)
    {
        printk(KERN_INFO "Cannot add the device to the system\n");
        goto r_class;
    }

    /*Creating struct class*/
    if((dev_class = class_create(THIS_MODULE, "kunix_class")) == NULL)
    {
        printk(KERN_INFO "Cannot create the struct class\n");
        goto r_class;
    }

    /*Creating device*/
    if((device_create(dev_class, NULL, dev, NULL, "kunix_device") == NULL))
    {
        printk(KERN_INFO "Cannot create the Device \n");
        goto r_device;
    }

    printk(KERN_INFO "Device Driver Insert... Done!!\n");
    return 0;

r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev, 1);
    return -1;
}

/*
** Module exit function
*/
static void __exit kunix_driver_exit(void)
{
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&kunix_cdev);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Device Driver Remove... Done!!\n");
}

module_init(kunix_driver_init);
module_exit(kunix_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kunix <kunix@gmail.com>");
MODULE_DESCRIPTION("Simple Linux Device Driver (File Operations)");
MODULE_VERSION("1.3");
