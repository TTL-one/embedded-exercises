#include <linux/module.h>
#include <linux/platform_device.h>

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
#include <linux/gpio/consumer.h>
#include <linux/of.h>


/* 1. 确定主设备号                                                                 */
static int major = 0;
static struct class *led_class;
static struct gpio_desc *led_gpio;

static int chip_led_fops_open(struct inode *node, struct file *file){
    //完成 pin和gpio初始化
    gpiod_direction_output(led_gpio, 0);
}
static int chip_led_fops_write(struct file *file, const char __user *buf, size_t size, loff_t *offset){
    //完成 pin和gpio初始化
    char status;
    copy_from_user(&status, buf, 1);
    gpiod_set_value(led_gpio, status);
    //1 表示开灯 0表示关灯，而非高低电平 因为设备树中会设置HIGH为有效还是LOW为有效，1指的是有效
}
static int chip_led_fops_read(struct file *file, const char __user *buf, size_t size, loff_t *offset){
    //完成 pin和gpio初始化
    char status;
    gpiod_get_value(led_gpio,status);
    copy_to_user(buf,&status, 1);
}
static int chip_led_fops_release (struct inode *node, struct file *file)
{
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

static struct file_operations chip_led_fops={
    .owner	 = THIS_MODULE,
    .open = chip_led_fops_open,
    .write = chip_led_fops_write,
    .read = chip_led_fops_read,
    .release = chip_led_fops_release,
}

static int chip_led_gipo_driver_probe(struct platform_device *pdev){
    //获取gpio操作函数句柄，方便后续函数使用    设备树中为 led-gpios = <>
    led_gpio = gpiod_get(&pdev->dev, "led", 0);
    major = register_chrdev(0, "100ask_led", &chip_led_fops); 
    led_class = class_create(THIS_MODULE, "100ask_led_class");
    device_create(led_class, NULL, MKDEV(major, 0), NULL, "100ask_led%d", 0);
}

static int chip_led_gipo_driver_remove(struct platform_device *pdev){
	device_destroy(led_class, MKDEV(major, 0));
	class_destroy(led_class);
	unregister_chrdev(major, "100ask_led");
	gpiod_put(led_gpio);
}

static const struct of_device_id _100ask_leds[] = {
    {.compatible = "100ask_leddrv"},
    {}
}
static struct platform_driver chip_led_gpio_driver ={
    .probe = chip_led_gipo_driver_probe,
    .remove = chip_led_gipo_driver_remove,
    .driver = {
        .name = "100ask_led",
        .of_match_table = _100ask_leds,
    }
}

/* 2. 在入口函数注册platform_driver */
static int __init led_init(void)
{
    int err;
    
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	
    err = platform_driver_register(&chip_led_gpio_driver); 
	
	return err;
}

/* 3. 有入口函数就应该有出口函数：卸载驱动程序时，就会去调用这个出口函�? *     卸载platform_driver
 */
static void __exit led_exit(void)
{
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

    platform_driver_unregister(&chip_led_gpio_driver);
}


/* 7. 其他完善：提供设备信息，自动创建设备节点                                     */

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");


