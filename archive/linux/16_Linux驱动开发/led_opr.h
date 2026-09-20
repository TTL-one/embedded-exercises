#ifndef _LED_OPR_H
#define _LED_OPR_H
struct led_operations {
    int num;
    int (*init)(int which); /*初始化LED，which表示具体的LED编号*/
    char (*read)(int which); /*读取LED状态，which表示具体的LED编号，返回值表示LED状态*/
    int (*ctl)(int which, int status); /*控制LED，which表示具体的LED编号，status表示LED状态*/
};

struct led_operations *get_led_opr(void);
#endif