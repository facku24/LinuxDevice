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
  *************************************************************************************/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>         //kmalloc()
#include <linux/uaccess.h>      //copy_to/from_user()

#define mem_size        1024    //Memory Size

dev_t dev = 0;
static struct class * dev_class;
static struct cdev kunix_cdev;
uint8_t *kernel_buffer;

/*
** Function Prototypes
*/
static int      __init kunix_driver_init(void);
static void     __exit kunix_driver_exit(void);
static int      kunix_open(struct inode *inode, struct file *file);
static int      kunix_release(struct inode *inode, struct file *file);
static ssize_t  kunix_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t  kunix_write(struct file *filp, const char *buf, size_t len, loff_t *off);

/*
** File Operations structure
*/
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
    /*Creating Physical memory*/
    if((kernel_buffer = kmalloc(mem_size, GFP_KERNEL)) == 0)
    {
        printk(KERN_INFO "Cannot allocate memory in kernel\n");
        return -1;
    }
    printk(KERN_INFO "Device File Opened...!!!\n");
    return 0;
}

/*
** This functio will be called when we close the Device file
*/
static int kunix_release(struct inode *inode, struct file *file)
{
    kfree(kernel_buffer);
    printk(KERN_INFO "Device File Closed...!!!\n");
    return 0;
}

/*
** This functions will be called when we read the Device file
*/
static ssize_t kunix_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    //Copy the data from the kernel space to the user-space
    copy_to_user(buf, kernel_buffer, mem_size);
    printk(KERN_INFO "Data Read : Done!\n");
    return mem_size;
}

/*
** This function will be called when we write the Device file
*/
static ssize_t kunix_write(struct file *filp, const char *buf, size_t len, loff_t *off)
{
    //Copy the data to the kernel space from the user-space
    copy_from_user(kernel_buffer, buf, len);
    printk(KERN_INFO "Data Write : Done!\n");
    return len;
}

/*
** MOdule Init function
*/
static int __init kunix_driver_init(void)
{
    /*Allocatin Major Number*/
    if((alloc_chrdev_region(&dev, 0, 1, "kunix_dev")) < 0)
    {
        printk(KERN_INFO "Cannot allocate major number\n");
        return -1;
    }
    printk(KERN_INFO "Major = %d Minor %d \n", MAJOR(dev), MINOR(dev));

    /*Creating cdev structure*/
    cdev_init(&kunix_cdev, &fops);

    /*Adding character device to the system*/
    if((cdev_add(&kunix_cdev, dev, 1)) < 0)
    {
        printk(KERN_INFO "Cannot create the struct class\n");
        goto r_class;
    }

    /*Creating struct class*/
    if((dev_class = class_create(THIS_MODULE, "kunix_class")) == NULL)
    {
        printk(KERN_INFO "Cannot create the struct class\n");
        goto r_class;
    }

    /*Creating device*/
    if((device_create(dev_class, NULL, dev, NULL, "kunix_device")) == NULL)
    {
        printk(KERN_INFO "Cannot create the Device\n");
        goto r_device;
    }
    printk(KERN_INFO "Device Driver Inserte...Done!!!\n");
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
    printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}

module_init(kunix_driver_init);
module_exit(kunix_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kunix <kunix@gmail.com>");
MODULE_DESCRIPTION("Simple Linux Device Driver (Real Linux Device Driver)");
MODULE_VERSION("1.4");