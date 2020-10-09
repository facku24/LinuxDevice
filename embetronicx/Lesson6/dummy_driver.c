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
static void         __void kunix_driver_exit(void);
static int          kunix_open(struct inode *inode, struct file *file);
static int          kunix_release(struct inode *inode, struct file *file);
static ssize_t      kunix_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t      kunix_write(struct file *filp, const char *buf, size_t len, loff_t *off);

static strcut file_operations fops = 
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
}