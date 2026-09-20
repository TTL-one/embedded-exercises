#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include "font_8x16.h"

static int fd_fb;   //LCD framebuffer文件描述符
static struct fb_var_screeninfo var;	// LCD 的参数
static int screen_size;  // 屏幕分辨率
static unsigned int line_width; //LCD 每行字节数
static unsigned int pixel_width; //LCD 每个像素占用的字节数
static unsigned char *fb_base; // 映射到内存的LCD起始地址

#define BACKGROUND 0x000000 /*背景色黑色*/

/*======================================================================
 * 两种操作方式的像素写入函数声明
 *======================================================================*/
void lcd_put_pixel(int x, int y, unsigned int color);  // 方式A: mmap指针写入
void lcd_put_char(int x,int y,char c,unsigned int color); // 指定位置显示一个字符
void lcd_put_str(int x, int y, const char * str,unsigned int color);    // 指定位置显示一个字符串

void lcd_put_pixel(int x, int y, unsigned int color){
    /*计算像素在显存中的字节偏移量*/
    unsigned int position = y * line_width + x * pixel_width;

    /*从color中提取RGB分量（假设传入格式为0x00RRGGBB）*/
    unsigned char red   = (color >> 16) & 0xFF;
    unsigned char green = (color >> 8)  & 0xFF;
    unsigned char blue  =  color        & 0xFF;

    if(var.bits_per_pixel == 32){
        /*RGB8888: 每个像素占4字节，低24位存RGB，高8位为0*/
        unsigned int *pixel = (unsigned int *)(fb_base + position);
        *pixel = (red << 16) | (green << 8) | blue;
    } else if(var.bits_per_pixel == 16){
        /*RGB565: 每个像素占2字节，R占5位(bit11-15)，G占6位(bit5-10)，B占5位(bit0-4)*/
        unsigned short *pixel = (unsigned short *)(fb_base + position);
        unsigned short r = red >> 3;    /*8位压缩到5位：red>>3，范围变成0-31*/
        unsigned short g = green >> 2;  /*8位压缩到6位：green>>2，范围变成0-63*/
        unsigned short b = blue >> 3;   /*8位压缩到5位：blue>>3，范围变成0-31*/
        *pixel = (r << 11) | (g << 5) | b;
    } else if(var.bits_per_pixel == 8){
        /*调色板模式：color是调色板索引，直接写索引值*/
        unsigned char *pixel = fb_base + position;
        *pixel = color & 0xFF;
    }
}

void lcd_put_char(int x,int y,char c,unsigned int color){
    //1. 获取点阵数据
    unsigned char *dots = (unsigned char *)&fontdata_8x16[c*16]; //每个字符16字节
    //2. 遍历点阵数据，绘制字符
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 8; j++){
            if(dots[i] & (1 << (7-j))){
                lcd_put_pixel(x+j, y+i, color);
            }else{
                lcd_put_pixel(x+j, y+i, BACKGROUND); //背景色黑色
            }
        }
    }
}

void lcd_put_str(int x, int y, const char * str,unsigned int color){
    while(*str){
        lcd_put_char(x, y, *str, color);
        x += pixel_width; //每个字符宽8像素
        if(x>=line_width){
            x = 0;
            y += 16; //每个字符高16像素
        }
        str++;
    }
}

/*======================================================================
 * main函数：对比两种方式的用法
 *======================================================================*/
int main(int argc, char ** argv){
    /*打开framebuffer设备文件*/
    fd_fb = open("/dev/fb0", O_RDWR);
    if(fd_fb < 0){
        perror("open /dev/fb0");
        return -1;
    }

    /*获取LCD可变参数（屏幕分辨率、颜色位数等）*/
    if(ioctl(fd_fb, FBIOGET_VSCREENINFO, &var)){
        perror("ioctl FBIOGET_VSCREENINFO");
        return -1;
    }

    /*计算屏幕尺寸参数*/
    pixel_width = var.bits_per_pixel / 8;       /*每个像素字节数: 4(RGB32)或2(RGB565)或1*/
    line_width  = var.xres * pixel_width;     /*每行字节数*/
    screen_size = var.xres * var.yres * pixel_width; /*全屏字节数*/

    printf("LCD: %dx%d, %d bpp, line_width=%d, screen_size=%d bytes\n",
           var.xres, var.yres, var.bits_per_pixel, line_width, screen_size);

    fb_base = (unsigned char *)mmap(NULL, screen_size,
                                     PROT_READ | PROT_WRITE, MAP_SHARED,
                                     fd_fb, 0);
    if(fb_base == (unsigned char *)-1){
        perror("mmap");
        return -1;
    }

    /*用方式A清屏（直接memset这段虚拟内存）*/
    memset(fb_base, 0, screen_size);

    lcd_put_char(var.xres/2, var.yres/2, 'A', 0xFFFFFF); //在屏幕中间显示白色字符'A'

    munmap(fb_base, screen_size);
    close(fd_fb);
    return 0;
}
