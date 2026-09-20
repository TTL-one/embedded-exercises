#include <linux/module.h>

#include <linux/fs.h>
#include <linux/io.h>
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

/* ========== 寄存器布局 ========== */

struct imx6ull_gpio {
    volatile unsigned int dr;
    volatile unsigned int gdir;
    volatile unsigned int psr;
    volatile unsigned int icr1;
    volatile unsigned int icr2;
    volatile unsigned int imr;
    volatile unsigned int isr;
    volatile unsigned int edge_sel;
};

/* ========== GPIO 位操作宏 ========== */

#define GPIO_DR_SET(gpio, pin)     ((gpio)->dr |=  (1 << (pin)))
#define GPIO_DR_CLR(gpio, pin)     ((gpio)->dr &= ~(1 << (pin)))
#define GPIO_GDIR_OUT(gpio, pin)   ((gpio)->gdir |= (1 << (pin)))

/* ========== 硬件基址与偏移 ========== */

#define CCM_BASE           0x20C4000
#define CCM_CCGR1_OFF      0x6C
#define IOMUXC_BASE        0x20E0000
#define IOMUXC_SNVS_BASE   0x2290000
#define GPIO1_BASE         0x209C000
#define GPIO5_BASE         0x20AC000

/* GPIO1 的 MUX 寄存器偏移 = 0x5c + pin * 4 */
#define IOMUX_GPIO1_MUX(pin)  (0x5c + (pin) * 4)
#define IOMUX_SNVS_TAMPER3    0x14

/* ========== LED 描述符 ========== */

struct led_desc {
    struct imx6ull_gpio __iomem *gpio;
    int pin;
    unsigned int iomux_base;
    int iomux_off;
    int ccgr_shift;
};

static volatile unsigned int *ccm_ccgr1;
static volatile unsigned int *iomuxc;
static volatile unsigned int *iomuxc_snvs;

static struct led_desc leds[] = {
    { NULL, 3, IOMUXC_SNVS_BASE, IOMUX_SNVS_TAMPER3,   30 },  /* GPIO5_IO03 */
    { NULL, 3, IOMUXC_BASE,      IOMUX_GPIO1_MUX(3),   26 },  /* GPIO1_IO03 */
    { NULL, 5, IOMUXC_BASE,      IOMUX_GPIO1_MUX(5),   26 },  /* GPIO1_IO05 */
    { NULL, 6, IOMUXC_BASE,      IOMUX_GPIO1_MUX(6),   26 },  /* GPIO1_IO06 */
};
#define NUM_LEDS  ARRAY_SIZE(leds)

/* ========== 驱动函数 ========== */

static int board_demo_led_init(int which)
{
    struct led_desc *led;

    if (which < 0 || which >= NUM_LEDS)
        return -EINVAL;

    led = &leds[which];

    /* 一劳永逸映射全部寄存器 */
    if (!ccm_ccgr1) {
        ccm_ccgr1    = ioremap(CCM_BASE + CCM_CCGR1_OFF, 4);
        iomuxc       = ioremap(IOMUXC_BASE, 4);
        iomuxc_snvs  = ioremap(IOMUXC_SNVS_BASE, 4);

        leds[0].gpio = ioremap(GPIO5_BASE, sizeof(struct imx6ull_gpio));
        leds[1].gpio = ioremap(GPIO1_BASE, sizeof(struct imx6ull_gpio));
        leds[2].gpio = ioremap(GPIO1_BASE, sizeof(struct imx6ull_gpio));
        leds[3].gpio = ioremap(GPIO1_BASE, sizeof(struct imx6ull_gpio));
    }

    /* 1. 开时钟 */
    *ccm_ccgr1 |= (3 << led->ccgr_shift);

    /* 2. 设 MUX 为 GPIO 模式 (ALT5) */
    writel(5, led->iomux_base + led->iomux_off);

    /* 3. 设方向为输出 */
    GPIO_GDIR_OUT(led->gpio, led->pin);

    return 0;
}

static int board_demo_led_ctl(int which, char status)
{
    struct led_desc *led;

    if (which < 0 || which >= NUM_LEDS)
        return -EINVAL;

    led = &leds[which];

    /* 低电平点亮 */
    if (status)
        GPIO_DR_CLR(led->gpio, led->pin);
    else
        GPIO_DR_SET(led->gpio, led->pin);

    return 0;
}

static struct led_operations board_demo_led_opr = {
    .num  = NUM_LEDS,
    .init = board_demo_led_init,
    .ctl  = board_demo_led_ctl,
};

struct led_operations *get_board_led_opr(void)
{
    return &board_demo_led_opr;
}
