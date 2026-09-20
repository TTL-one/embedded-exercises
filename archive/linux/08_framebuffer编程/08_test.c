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
#include <sys/types.h>
#include <sys/stat.h>


static int fd_fb;   //LCD framebuffer文件描述符
static struct fb_var_screeninfo var;	// LCD 的参数
static int screen_size;  // 屏幕分辨率
static unsigned int line_width; //LCD 每行字节数
static unsigned int pixel_width; //LCD 每个像素占用的字节数
static unsigned char *fb_base; // 映射到内存的LCD起始地址

#define BACKGROUND 0x000000

void lcd_put_pixel(int x, int y, unsigned int color);  // 方式A: mmap指针写入
void lcd_put_char(int x,int y,char c,unsigned int color); // 指定位置显示一个字符
void lcd_put_str(int x, int y, const char * str,unsigned int color);    // 指定位置显示一个字符串
void lcd_put_chinese(int x,int y,const char *str,unsigned int color); //指定位置显示一个中文字符串

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

void lcd_put_char(int x, int y, unsigned char c, unsigned int color) {
    // 假设 fontdata_8x16 包含 0x00-0xFF，如果从空格开始则改为 c - ' '
    unsigned char *dots = (unsigned char *)&fontdata_8x16[c * 16];
    for (int i = 0; i < 16; i++) {
        unsigned char line = dots[i];
        for (int j = 0; j < 8; j++) {
            if (line & (1 << (7 - j)))
                lcd_put_pixel(x + j, y + i, color);
            else
                lcd_put_pixel(x + j, y + i, BACKGROUND);
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

void lcd_put_chinese(int x,int y,const char *str,unsigned int color){
    int index = (str[0]-0xa1) * 94 + (str[1]-0xa1); //计算GB2312编码的字符索引
    unsigned char * dots = (unsigned char *)&fontdata_8x16[index*32]; //中文字符点阵数据从索引256开始
    for(int i=0;i<16;i++){
        for(int j = 0;j<2;j++){
            for(int k = 0;k<8;k++){
                if(dots[j*i]>>(7-k) & 0x01){
                    lcd_put_pixel(x+j*8+k, y+i, color);
                }else{
                    lcd_put_pixel(x+j*8+k, y+i, BACKGROUND); //背景色黑色
                }
            }
        }
    }
}

static char * hzk16_data = NULL; // 存储汉字点阵数据的指针

void lcd_put_chinese(int x, int y, const unsigned char *gb2312_code, unsigned int color) {
    unsigned char qu = gb2312_code[0] - 0xA0; // 区码 (1-94)
    unsigned char wei = gb2312_code[1] - 0xA0; // 位码
    unsigned int index = (qu - 1) * 94 + (wei - 1); // 区位索引 (0-6767)
    const unsigned char *dots = &hzk16_data[index * 32]; // 每个汉字32字节

    for (int i = 0; i < 16; i++) {
        unsigned char line1 = dots[i * 2];      // 左半边 8 位
        unsigned char line2 = dots[i * 2 + 1];  // 右半边 8 位
        for (int j = 0; j < 8; j++) {
            if (line1 & (1 << (7 - j)))
                lcd_put_pixel(x + j, y + i, color);
            else
                lcd_put_pixel(x + j, y + i, BACKGROUND);
        }
        for (int j = 0; j < 8; j++) {
            if (line2 & (1 << (7 - j)))
                lcd_put_pixel(x + 8 + j, y + i, color);
            else
                lcd_put_pixel(x + 8 + j, y + i, BACKGROUND);
        }
    }
}

void init_hzk16_data() {
    int fd = open("HZK16",O_RDONLY);
    if(fd<0){
        perror("open HZK16");
        return;
    }
    //screen_size 不对， sreen_size是用于写入framebuffer的，而这里是读出文件中的数组，所以应该是文件大小
    //       int fstat(int fd, struct stat *statbuf);
    //       fstat() is identical to stat(), except that the file about which infor‐
    //   mation is to be retrieved is specified by the file descriptor fd.
    struct stat st;
        /*
                struct stat {
                    dev_t     st_dev;         /* ID of device containing file
                    ino_t     st_ino;         /* Inode number
                    mode_t    st_mode;        /* File type and mode
                    nlink_t   st_nlink;       /* Number of hard links
                    uid_t     st_uid;         /* User ID of owner
                    gid_t     st_gid;         /* Group ID of owner
                    dev_t     st_rdev;        /* Device ID (if special file)
                    off_t     st_size;        /* Total size, in bytes
                    blksize_t st_blksize;     /* Block size for filesystem I/O
                    blkcnt_t  st_blocks;      /* Number of 512B blocks allocated

        */
    fstat(fd, &st);
    hzk16_data = (unsigned char *)mmap(NULL,st.st_size,PROT_READ,MAP_SHARED,fd,0);
	if (hzk16_data == (unsigned char *)-1)
	{
		printf("can't mmap for hzk16\n");
		return -1;
	}
    close(fd);
}

void lcd_show_str(int x,int y,const char *str,unsigned int color){
    while(*str){
        if(*str & 0x80){ //如果是中文字符
            lcd_put_chinese(x, y, str, color);
            x += 16; //中文字符宽16像素
            str += 2; //GB2312编码占2字节
        }else{ //ASCII字符 GB2312编码应该包括了所有ASCII字符所以可以直接使用
            lcd_put_char(x, y, *str, color);
            x += 8; //ASCII字符宽8像素
            str++;
        }
        if(x>=line_width){
            x = 0;
            y += 16; //每行高16像素
        }
    }
}

void lcd_show_str(int x, int y, const char *str, unsigned int color) {
    int cur_x = x, cur_y = y;
    while (*str) {
        if ((*str & 0x80) && (*(str+1) != '\0')) { // 双字节中文字符
            lcd_put_chinese(cur_x, cur_y, (unsigned char *)str, color);
            cur_x += 16; // 汉字宽16像素
            str += 2;
        } else {
            lcd_put_char(cur_x, cur_y, *str, color);
            cur_x += 8;
            str++;
        }
        // 换行判断：用实际屏幕宽度 var.xres
        if (cur_x + 8 > var.xres) { // 下一字符可能超出右边界
            cur_x = 0;
            cur_y += 16;
        }
        // 超出高度也可以处理，此处略
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
