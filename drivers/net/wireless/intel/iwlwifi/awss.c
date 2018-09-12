#include <linux/module.h>
#include <linux/pm_runtime.h>
#include <linux/pci.h>
#include <linux/pci-aspm.h>
#include <linux/acpi.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#define AWSS_NAME  "awss"
#if 0
int awss_open(struct inode *inode, struct file *filp)
{
    printk("-----------------------------awss_open");
    return 0;
}

int awss_release(struct inode *inode, struct file *filp)
{
    printk("-----------------------------awss_release");
    return 0;
}
#endif
static long awss_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    printk("-----------------------------awss_ioctl");
    switch (cmd) {
        default:
            printk("===========================================%d", cmd);
            break;
    }
    return 0;
}

static struct file_operations awss_fops = {
    .owner = THIS_MODULE,
#if 0
    .open = awss_open,
    .release = awss_release,
#endif
    .unlocked_ioctl = awss_ioctl,
};

static dev_t awss_dev;
static struct cdev *awss_cdev;
static int awss_major = 0;
static int awss_minor = 0;

int awss_create_device(void)
{
    int ret;

    printk("awss_create_deice");
    ret = alloc_chrdev_region(&awss_dev, 0, 1, AWSS_NAME);
    printk("alloc chrdev region ret:%d", ret);
    if (ret < 0)
        return ret;
    awss_major = MAJOR(awss_dev);
    awss_minor = MINOR(awss_dev);
    printk("awss_major:%d, awss_minor:%d", awss_major, awss_minor);

    awss_cdev = cdev_alloc();
    printk("awss_cdev:%p", awss_cdev);
    if (awss_cdev == NULL)
        return -1;
    cdev_init(awss_cdev, &awss_fops);
    awss_cdev->owner = THIS_MODULE;
    awss_cdev->ops = &awss_fops;
    ret = cdev_add(awss_cdev, awss_dev, 1);
    printk("cdev add %d", ret);
    return 0;
}
EXPORT_SYMBOL(awss_create_device);

int awss_destroy_device(void)
{
    awss_dev = MKDEV(awss_major, awss_minor);
    printk("destroy awss device");
    cdev_del(awss_cdev);
    unregister_chrdev_region(awss_dev, 1);
    awss_cdev = NULL;
    awss_major = 0;
    awss_minor = 0;
    return 0;
}

EXPORT_SYMBOL(awss_destroy_device);
