
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
#include <linux/platform_device.h>

#include "led_resource.h"


static void led_dev_release(struct device *dev)
{
}

/*      这是参照内核一些代码的实现完成的
 *      一个很重要的思路就是，别人定义好的类型你可以直接用，自己对应好就行，不需要自己再创建新的类
 *      比如这里定义我们的LED为IORESOURCE_IRQ类型，因为官方结构体资源中没有LED类型，我们指定为这个
 *      在自己的代码中使用时只需要判断是不是IORESOURCE_IRQ就是LED，自己做一个默认规定即可。
 *      而且resource不止是有这三个内容，而且start原意是起始地址 这里我们设置为LED的GPIO编号，也是自己的默认规定
 *      在硬件具体实现 即平台驱动中是这样的
 *      platform_get_resource(pdev, IORESOURCE_IRQ, i++);
 *      所以都是自己的规定
 */ 
static struct resource resources[] = {
        {
                .start = GROUP_PIN(3,1),
                .flags = IORESOURCE_IRQ,
                .name = "100ask_led_pin",
        },
        {
                .start = GROUP_PIN(5,8),
                .flags = IORESOURCE_IRQ,
                .name = "100ask_led_pin",
        },
};
/*
 *      平台设备也是内核中的定义，和resource一样，也是自己对应即可
 *      采用内核的架构进行设计就是这样的 要求资源 硬件具体实现 还有驱动分离开来
 *      当前文件就是资源的定义和注册，也要实现入口函数和出口函数
 *      具体实现的函数也是一样，要实现入口函数和出口函数，这样两者就可以在内核里通过name匹配完成资源和具体实现的绑定
 *      release函数就算为空也需要定义
        static struct platform_driver chip_demo_gpio_driver = {
        .probe      = chip_demo_gpio_probe,
        .remove     = chip_demo_gpio_remove,
        .driver     = {
                .name   = "100ask_led",
        },
        };
 */

static struct platform_device board_A_led_dev = {
        .name = "100ask_led",
        .num_resources = ARRAY_SIZE(resources),
        .resource = resources,
        .dev = {
                .release = led_dev_release,
         },
};

static int __init led_dev_init(void)
{
    int err;
    
    err = platform_device_register(&board_A_led_dev);   
    
    return 0;
}

static void __exit led_dev_exit(void)
{
    platform_device_unregister(&board_A_led_dev);
}

module_init(led_dev_init);
module_exit(led_dev_exit);

MODULE_LICENSE("GPL");

