/*************************************************************************************
 * FILE:            driver.c
 * DETAILS:     Simple linux driver (Real Linux Device Driver)
 * AUTHOR:      Kunix
 * COMPILE:  	make
 * CLEAN: 		make clean
 * INSTALL: 	sudo insmod <driver_name>.ko
 * CHECK:		lsmod | grep <driver_name>
 * VERIFY:		dmesg
 * REMOVE:		sudo rmmod <driver_name> | <driver_name>.ko
 * INFO: 		modinfo <driver_name>.ko
 * CHECK:       ls -l /dev/ | grep <driver_name>
 * TEST:        sudo bash -c 'echo 1 > /dev/kunix_device'
 * ***********************************************************************************/
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/slab.h>          //kmalloc()
#include<linux/uaccess.h>       //copy_to/from_user()
#include<linux/ioctl.h>

// Define the commands in the driver
#define WR_VALUE _IOW('a', 'a', int32_t*)
#define RD_VALUE _IOR('a', 'b', int32_t*)

int32_t value = 0;

dev_t dev = 0;

static struct class *dev_class;
static struct cdev ext_cdev;

/*
** Functions Prototypes
*/
static int      __init ext_driver_init(void);
static void     __exit ext_driver_exit(void);
static int      ext_open(struct inode * inode, struct file *file);
static int      ext_release(struct inode * inode, struct file *file);
static ssize_t  ext_read(struct file *filp, char *buf, size_t len, loff_t *off);
static ssize_t  ext_write(struct file *filp, const char *buf, size_t len, loff_t *off);
static long     ext_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

/*
** File operation structure
*/
static struct file_operations fops =
{
    .owner          = THIS_MODULE,
    .read           = ext_read,
    .write          = ext_write,
    .open           = ext_open,
    .unlocked_ioctl = ext_ioctl,
    .release        = ext_release,
};

/*
** This function will be called when we open the Device file
*/
static int ext_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device File Opened...!!!\n");
    return 0;
}

/*
** This function will be called when we close the Device file
*/
static int ext_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device File Closed...!!!\n");
    return 0;
}

/*
** This function will be called when we read the Device file
*/
static ssize_t ext_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Read Function\n");
    return 0;
}

/*
** This function will be called when we write the Device file
*/
static ssize_t ext_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Write function\n");
    return 0;
}

/*
** This function will be called when we write IOCTL on the Device file
*/
static long ext_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch(cmd) {
        case WR_VALUE:
            copy_from_user(&value, (int32_t*) arg, sizeof(value));
            printk(KERN_INFO "Value = %d\n", value);
            break;
        case RD_VALUE:
            copy_to_user((int32_t*) arg, &value, sizeof(value));
            break;
    }
    return 0;
}

/*
** Module Init function
*/
static int __init ext_driver_init(void)
{
    /*Allocating Major Number*/
    if((alloc_chrdev_region(&dev, 0, 1, "ext_Dev") < 0)){
        printk(KERN_INFO "Cannot allocate major number \n");
        return -1;
    }
    printk(KERN_INFO "Major = %d Minor = %d\n", MAJOR(dev), MINOR(dev));

    /*Creating cdev structure*/
    cdev_init(&ext_cdev, &fops);

    /*Adding character device to the system*/
    if((cdev_add(&ext_cdev, dev, 1)) < 0) {
        printk(KERN_INFO "Cannot add the device to the system\n");
        goto r_class;
    }

    /*Creating struct class*/
    if((device_create(dev_class, NULL, dev, NULL, "ext_device")) == NULL){
        printk(KERN_INFO "Cannot create the Device 1\n");
        goto r_device;
    }
    printk(KERN_INFO "Device Driver Insert... Done!!!\n");
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
static void __exit ext_driver_exit(void)
{
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&ext_cdev);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Device Driver Remove... Done!!!\n");
}

module_init(ext_driver_init);
module_exit(ext_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kunix");
MODULE_DESCRIPTION("Simple Linux Device Driver (IOCTL)");
MODULE_VERSION("1.5");


