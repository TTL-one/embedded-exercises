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

#include "led_opr.h"
#include "leddrv.h"
#include "led_resource.h"

static int g_ledpins[100];
static int g_ledcnt = 0;

static int board_demo_led_init (int which) /* 初始化LED, which-哪个LED */       
{   
    //printk("%s %s line %d, led %d\n", __FILE__, __FUNCTION__, __LINE__, which);
    
    printk("init gpio: group %d, pin %d\n", GROUP(g_ledpins[which]), PIN(g_ledpins[which]));
    switch(GROUP(g_ledpins[which]))
    {
        case 0:
        {
            printk("init pin of group 0 ...\n");
            break;
        }
        case 1:
        {
            printk("init pin of group 1 ...\n");
            break;
        }
        case 2:
        {
            printk("init pin of group 2 ...\n");
            break;
        }
        case 3:
        {
            printk("init pin of group 3 ...\n");
            break;
        }
    }
    
    return 0;
}

static int board_demo_led_ctl (int which, char status) /* 控制LED, which-哪个LED, status:1-亮,0-灭 */
{
    //printk("%s %s line %d, led %d, %s\n", __FILE__, __FUNCTION__, __LINE__, which, status ? "on" : "off");
    printk("set led %s: group %d, pin %d\n", status ? "on" : "off", GROUP(g_ledpins[which]), PIN(g_ledpins[which]));

    switch(GROUP(g_ledpins[which]))
    {
        case 0:
        {
            printk("set pin of group 0 ...\n");
            break;
        }
        case 1:
        {
            printk("set pin of group 1 ...\n");
            break;
        }
        case 2:
        {
            printk("set pin of group 2 ...\n");
            break;
        }
        case 3:
        {
            printk("set pin of group 3 ...\n");
            break;
        }
    }

    return 0;
}

static struct led_operations board_demo_led_opr = {
    .init = board_demo_led_init,
    .ctl  = board_demo_led_ctl,
};

struct led_operations *get_board_led_opr(void)
{
    return &board_demo_led_opr;
}

/*
 *      平台设备的探测函数 平台设备也就是资源 如果两者匹配就调用probe
 *      也就是如果有资源，有gpio口就实现设备节点的创建
 *      设备节点的创建本来是由leddrv.c实现的，但是leddrv.c为了抽象一层，成为了通用的驱动，他不需要关心底层是什么
 *      led_class_create_device
 *      led_class_destroy_device
 *      这两个函数都是leddrv中提供的，他已经完成了类的创建，但是没有设备节点的实现
 */
static int chip_demo_gpio_probe(struct platform_device *pdev)
{
    struct resource *res;
    int i = 0;

    while (1)
    {
        res = platform_get_resource(pdev, IORESOURCE_IRQ, i++);
        if (!res)
            break;
        
        g_ledpins[g_ledcnt] = res->start;
        led_class_create_device(g_ledcnt);
        g_ledcnt++;
    }
    return 0;
    
}

static int chip_demo_gpio_remove(struct platform_device *pdev)
{
    struct resource *res;
    int i = 0;

    while (1)
    {
        res = platform_get_resource(pdev, IORESOURCE_IRQ, i);
        if (!res)
            break;
        
        led_class_destroy_device(i);
        i++;
        g_ledcnt--;
    }
    return 0;
}


static struct platform_driver chip_demo_gpio_driver = {
    .probe      = chip_demo_gpio_probe,
    .remove     = chip_demo_gpio_remove,
    .driver     = {
        .name   = "100ask_led",
    },
};

static int __init chip_demo_gpio_drv_init(void)
{
    int err;
    
    err = platform_driver_register(&chip_demo_gpio_driver); 
    register_led_operations(&board_demo_led_opr);
    
    return 0;
}

static void __exit lchip_demo_gpio_drv_exit(void)
{
    platform_driver_unregister(&chip_demo_gpio_driver);
}

module_init(chip_demo_gpio_drv_init);
module_exit(lchip_demo_gpio_drv_exit);

MODULE_LICENSE("GPL");

