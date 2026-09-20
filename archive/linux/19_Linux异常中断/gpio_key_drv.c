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
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/slab.h>


struct gpio_key{
	int gpio;
	struct gpio_desc *gpiod;
	int flag;
	int irq;
} ;

static struct gpio_key *gpio_keys_100ask;

static irqreturn_t gpio_key_isr(int irq, void *dev_id)
{
	struct gpio_key *gpio_key = dev_id;
	int val;
	val = gpiod_get_value(gpio_key->gpiod);
	

	printk("key %d %d\n", gpio_key->gpio, val);
	
	return IRQ_HANDLED;
}

/* 1. 从platform_device获得GPIO
 * 2. gpio=>irq
 * 3. request_irq
 */
static int gpio_key_probe(struct platform_device *pdev)
{
	int err;
	//和inode不一样，inode是虚拟文件系统的节点
	//device_node有一个根节点 of_root 能遍历所有的设备节点
	// pdev->dev.of_node → 当前 probe 对应的设备树节点
	// of_gpio_count(node) → 这个节点里 gpios 属性有多少个 pin
	struct device_node *node = pdev->dev.of_node;
	int count;
	int i;
	enum of_gpio_flags flag;
	unsigned flags = GPIOF_IN;
		
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

	count = of_gpio_count(node);	//得到有多少个GPIO设备
	//
	//gpios = <&gpio5 1 GPIO_ACTIVE_LOW
    //        &gpio4 14 GPIO_ACTIVE_LOW>; 
	if (!count)
	{
		printk("%s %s line %d, there isn't any gpio available\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}

	//malloc
	gpio_keys_100ask = kzalloc(sizeof(struct gpio_key) * count, GFP_KERNEL);
	for (i = 0; i < count; i++)
	{
		//获取gpio以及状态 node的第i个设备,gpio状态 
		gpio_keys_100ask[i].gpio = of_get_gpio_flags(node, i, &flag);
		if (gpio_keys_100ask[i].gpio < 0)
		{
			printk("%s %s line %d, of_get_gpio_flags fail\n", __FILE__, __FUNCTION__, __LINE__);
			return -1;
		}

		//将gpio编号转换为描述符
		gpio_keys_100ask[i].gpiod = gpio_to_desc(gpio_keys_100ask[i].gpio);
		gpio_keys_100ask[i].flag = flag & OF_GPIO_ACTIVE_LOW;

		if (flag & OF_GPIO_ACTIVE_LOW)
			flags |= GPIOF_ACTIVE_LOW;

		err = devm_gpio_request_one(&pdev->dev, gpio_keys_100ask[i].gpio, flags, NULL);

		//获取该gpio对应的虚拟中断号
		gpio_keys_100ask[i].irq  = gpio_to_irq(gpio_keys_100ask[i].gpio);
	}

	for (i = 0; i < count; i++)
	{
		//中断号、中断函数、中断触发类型、中断函数需要的参数
		err = request_irq(gpio_keys_100ask[i].irq, gpio_key_isr, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "100ask_gpio_key", &gpio_keys_100ask[i]);
	}
        
    return 0;
    
}

/* ★★★★★ 新方法 probe ★★★★★
 *
 * 使用 gpiod_get_index + gpiod_to_irq，不碰 GPIO 号
 *
 * 对比老方法 gpio_key_probe：
 *   老：get_gpio_flags → gpio_to_desc → request_one → gpio_to_irq
 *   新：gpiod_get_index                          → gpiod_to_irq
 *   gpiod_get_index 一行顶老方法四行
 */
static int gpio_key_probe_new(struct platform_device *pdev)
{
	int count;
	int i;
	int err;

	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

	/* of_gpio_count 仍需用——提前知道有多少个 GPIO */
	count = of_gpio_count(pdev->dev.of_node);
	if (!count) {
		printk("%s %s line %d, there isn't any gpio available\n",
		       __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}

	gpio_keys_100ask = kzalloc(sizeof(struct gpio_key) * count, GFP_KERNEL);

	for (i = 0; i < count; i++)
	{
		/* ★ gpiod_get_index(&pdev->dev, NULL, i, GPIOD_IN) ★
		 *
		 * 参数对照设备树：
		 *   gpios = <&gpio5 1 GPIO_ACTIVE_LOW
		 *            &gpio4 14 GPIO_ACTIVE_LOW>;
		 *
		 * ┌────────────────────┬────────────────────────────────────────────┐
		 * │ 参数                │ 含义                                        │
		 * ├────────────────────┼────────────────────────────────────────────┤
		 * │ &pdev->dev         │ 设备指针                                    │
		 * │                    │ gpio 子系统通过 dev->of_node 找到设备树节点  │
		 * │                    │ → 读该节点下的 "gpios" 属性                 │
		 * ├────────────────────┼────────────────────────────────────────────┤
		 * │ NULL (con_id)      │ 连接 ID，为空时从 "gpios" 属性取 GPIO      │
		 * │                    │ = "led" 时，从 "led-gpios" 属性取          │
		 * │                    │ 内核自动在 con_id 后面拼 "-gpios"            │
		 * ├────────────────────┼────────────────────────────────────────────┤
		 * │ i (index)          │ gpios 属性中第几个 GPIO specifier          │
		 * │                    │ i=0 → &gpio5 1 GPIO_ACTIVE_LOW            │
		 * │                    │ i=1 → &gpio4 14 GPIO_ACTIVE_LOW           │
		 * ├────────────────────┼────────────────────────────────────────────┤
		 * │ GPIOD_IN (flags)   │ 申请 + 设为输入（一步完成）                │
		 * │                    │ GPIOD_OUT_LOW  = 申请+输出+低电平         │
		 * │                    │ GPIOD_OUT_HIGH = 申请+输出+高电平         │
		 * │                    │ GPIOD_ASIS     = 只申请，不改方向          │
		 * └────────────────────┴────────────────────────────────────────────┘
		 *
		 * gpiod_get_index 内部等价于老方法四步：
		 *   ① of_get_gpio_flags(node, i, &flag)   ← 解析 phandle→找到 gpio_chip
		 *   ② gpio_to_desc(gpio号)                 ← 号→描述符
		 *   ③ gpiochip_request(标记 pin 被占用)    ← 相当于 devm_gpio_request_one
		 *   ④ gpiod_configure_direction(GPIOD_IN) ← 设方向为输入
		 *   全部封装在一行里
		 */
		gpio_keys_100ask[i].gpiod = gpiod_get_index(&pdev->dev, NULL, i, GPIOD_IN);
		if (IS_ERR(gpio_keys_100ask[i].gpiod)) {
			printk("%s %s line %d, gpiod_get_index fail\n",
			       __FILE__, __FUNCTION__, __LINE__);
			return PTR_ERR(gpio_keys_100ask[i].gpiod);
		}

		/* gpiod_to_irq → 用描述符拿虚拟中断号
		 * 比 gpio_to_irq(全局GPIO号) 少一次 radix tree 查找
		 */
		gpio_keys_100ask[i].irq = gpiod_to_irq(gpio_keys_100ask[i].gpiod);
	}

	for (i = 0; i < count; i++)
	{
		err = request_irq(gpio_keys_100ask[i].irq, gpio_key_isr,
		                  IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
		                  "100ask_gpio_key", &gpio_keys_100ask[i]);
		if (err) {
			printk("%s %s line %d, request_irq fail\n",
			       __FILE__, __FUNCTION__, __LINE__);
			return err;
		}
	}

	return 0;
}

static int probe(struct platform_device *pdev)
{
	/*
		gpio_keys_100ask {
			compatible = "100ask,gpio_key";
			gpios = <&gpio5 1 GPIO_ACTIVE_LOW
			&gpio4 14 GPIO_ACTIVE_LOW>;
			
			pinctrl-names = "default";
			pinctrl-0 = <&key1_100ask &key2_100ask>;
		};	
	*/
	//把每一个引脚都注册成中断
	//获取引脚个数
	int count = of_gpio_count(pdev->dev.of_node);
	for(int i = 0 ;i<count ;i++){
		//获取gpio描述符
		struct gpio_desc *gpiod = gpiod_get_index(&pdev->dev, NULL, i, GPIOD_IN);
		//获取gpio对应的中断号
		int irq = gpiod_to_irq(gpiod);
		//注册中断
		request_irq(irq, gpio_key_isr, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "100ask_gpio_key", gpiod);
	}
}
static int gpio_key_remove(struct platform_device *pdev)
{
	//int err;
	struct device_node *node = pdev->dev.of_node;
	int count;
	int i;

	count = of_gpio_count(node);
	for (i = 0; i < count; i++)
	{
		free_irq(gpio_keys_100ask[i].irq, &gpio_keys_100ask[i]);
	}
	kfree(gpio_keys_100ask);
    return 0;
}


static const struct of_device_id ask100_keys[] = {
    { .compatible = "100ask,gpio_key" },
    { },
};

/* 1. 定义platform_driver */
static struct platform_driver gpio_keys_driver = {
    .probe      = gpio_key_probe_new,  // 新方法：gpiod_get_index + gpiod_to_irq
    //.probe    = gpio_key_probe,      // 老方法：of_get_gpio_flags + 手工转
    .remove     = gpio_key_remove,
    .driver     = {
        .name   = "100ask_gpio_key",
        .of_match_table = ask100_keys,
    },
};

/* 2. 在入口函数注册platform_driver */
static int __init gpio_key_init(void)
{
    int err;
    
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	
    err = platform_driver_register(&gpio_keys_driver); 
	
	return err;
}

/* 3. 有入口函数就应该有出口函数：卸载驱动程序时，就会去调用这个出口函数
 *     卸载platform_driver
 */
static void __exit gpio_key_exit(void)
{
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

    platform_driver_unregister(&gpio_keys_driver);
}


/* 7. 其他完善：提供设备信息，自动创建设备节点                                     */

module_init(gpio_key_init);
module_exit(gpio_key_exit);

MODULE_LICENSE("GPL");


