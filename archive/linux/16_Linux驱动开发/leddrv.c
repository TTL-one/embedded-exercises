#include <linux/module.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>

#include "led_opr.h"


/* LED驱动程序的实现 
 0. 主设备号
 1. file_operations 结构体
 2. 注册 设备
 3. 卸载设备
 4. 一些额外声明
*/
static int major;
static struct class *led_class;
struct led_operations *led_opr;

static ssize_t led_drv_open(struct inode *node, struct file *file)
{
    // 按照LED编号 初始化LED 
    int minor = iminor(node);
    led_opr->init(minor);
    return 0;
}
static ssize_t led_drv_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos)
{
    char status = led_opr->read(MINOR(file_inode(file)->i_rdev));
    copy_to_user(buf, &status, 1);
    return 0;
}
static ssize_t led_drv_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos)
{
    char status;
    struct inode *inode = file_inode(file);
	int minor = iminor(inode);
    copy_from_user(&status, buf, 1);
    led_opr->ctl(minor, status);
    return 0;
}
static ssize_t led_drv_close(struct inode *node, struct file *file)
{
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

static struct file_operations led_drv = {
	.owner	 = THIS_MODULE,
	.open    = led_drv_open,
	.read    = led_drv_read,
	.write   = led_drv_write,
	.release = led_drv_close,
};

static int __init led_init(void)
{
    major = register_chardev(0,"led_drv",&led_drv);

    //创建设备节点,先创建类，再创建设备 与C++的类关系不大，这更像是文件目录结构
    led_class = class_create(THIS_MODULE,"led_drv_class");
	if (IS_ERR(led_class)) {
		printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
		unregister_chrdev(major, "led");
		return -1;
	}
    //实例化设备
    led_opr = get_led_opr();
    for(int i = 0; i<led_opr->num; i++){
        device_create(led_class, NULL, MKDEV(major, i), NULL, "led_drv%d", i);
    }
    return 0;
}

static void __exit led_exit(void)
{
    device_destroy(led_class, MKDEV(major, 0));
    class_destroy(led_class);
    unregister_chardev(major,"led_drv");
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
