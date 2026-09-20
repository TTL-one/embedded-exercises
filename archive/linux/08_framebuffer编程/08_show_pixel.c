#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

static int fd_fb;   //LCD framebuffer文件描述符
static struct fb_var_screeninfo var;	// LCD 的参数
static int screen_size;  // 屏幕分辨率
static unsigned int line_width; //LCD 每行字节数
static unsigned int pixel_width; //LCD 每个像素占用的字节数
static unsigned char *fb_base; // 映射到内存的LCD起始地址

/*======================================================================
 * 两种操作方式的像素写入函数声明
 *======================================================================*/
void lcd_put_pixel_mmap(int x, int y, unsigned int color);  // 方式A: mmap指针写入
void lcd_put_pixel_file(int x, int y, unsigned int color);  // 方式B: lseek+write文件操作

/*======================================================================
 * 方式A: 通过mmap映射后的指针直接写入
 *
 * 原理：mmap把显存映射到进程虚拟地址空间，返回起始地址fb_base。
 *       此后fb_base[i]就和操作普通数组一样，MMU自动完成虚拟到物理的翻译。
 *
 * 优点：指针运算，极快；适合频繁大量像素写入（画线、画圆、显示图片等）
 * 缺点：需要理解虚拟地址/物理地址/MMU映射的概念
 *======================================================================*/
void lcd_put_pixel_mmap(int x, int y, unsigned int color){
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

/*======================================================================
 * 方式B: 通过lseek定位 + write写入（完全用文件操作，不用指针）
 *
 * 原理：Framebuffer在Linux里就是一个文件，文件的起始位置就是显存的起始地址。
 *       用lseek把"文件指针"定位到像素对应的字节偏移量，
 *       然后write把像素数据写进去。
 *
 *       lseek(fd, offset, SEEK_SET) → 把读写位置定位到offset字节处
 *       write(fd, buf, count)       → 从当前位置写入count字节
 *
 * 对比mmap：
 *   内存中一个像素:
 *     mmap方式:  fb_base[position] = data;        // 1条指令，CPU直接写内存
 *     文件方式:  lseek(fd, position); write(fd, &data, sizeof(data));
 *                // 2次系统调用，涉及内核态切换，硬件实际行为完全相同
 *
 * 优点：概念简单，任何文件都适用；适合偶尔写入少量像素的场景
 * 缺点：每次写入都是一次系统调用+内核态切换，开销远大于指针直接写
 *======================================================================*/
void lcd_put_pixel_file(int x, int y, unsigned int color){
    /*计算像素在显存中的字节偏移量（和mmap方式完全一样）*/
    unsigned int position = y * line_width + x * pixel_width;

    /*提取RGB分量（和mmap方式完全一样）*/
    unsigned char red   = (color >> 16) & 0xFF;
    unsigned char green = (color >> 8)  & 0xFF;
    unsigned char blue  =  color        & 0xFF;

    if(var.bits_per_pixel == 32){
        /*把4字节的像素数据先存入临时变量*/
        unsigned int pixel_data = (red << 16) | (green << 8) | blue;
        /*lseek把文件指针定位到目标像素的字节偏移处，然后write写入4字节*/
        lseek(fd_fb, position, SEEK_SET);
        write(fd_fb, &pixel_data, 4);
    } else if(var.bits_per_pixel == 16){
        unsigned short pixel_data = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
        lseek(fd_fb, position, SEEK_SET);
        write(fd_fb, &pixel_data, 2);
    } else if(var.bits_per_pixel == 8){
        unsigned char pixel_data = color & 0xFF;
        lseek(fd_fb, position, SEEK_SET);
        write(fd_fb, &pixel_data, 1);
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

    /*======================================================================
     * 演示方式A: mmap
     *
     * mmap把整个framebuffer(大小=screen_size字节)映射到进程地址空间，
     * 返回虚拟地址fb_base。
     * 此后: fb_base[0] ~ fb_base[screen_size-1] 就是显存的每一字节，
     *       直接读写即可，MMU自动完成虚拟地址到物理地址的翻译。
     *======================================================================*/
    fb_base = (unsigned char *)mmap(NULL, screen_size,
                                     PROT_READ | PROT_WRITE, MAP_SHARED,
                                     fd_fb, 0);
    if(fb_base == (unsigned char *)-1){
        perror("mmap");
        return -1;
    }

    /*用方式A清屏（直接memset这段虚拟内存）*/
    memset(fb_base, 0, screen_size);

    /*用方式A显示一个像素点（红色，位于屏幕中央）*/
    lcd_put_pixel_mmap(var.xres/2, var.yres/2, 0x00FF0000);

    printf("方式A(mmap): 在屏幕中央显示一个红点完成\n");

    /*======================================================================
     * 演示方式B: lseek + write
     *
     * 不需要mmap，直接用文件操作。
     * lseek(fd, offset, SEEK_SET) 把文件读写指针定位到offset字节处；
     * write(fd, buf, count)         从该位置写入count字节。
     *
     * 注意：open返回的文件描述符fd，在mmap后仍然有效，两者可以混用。
     *       但同一个像素位置，mmap方式和write方式同时写会互相覆盖（后面覆盖前面的）。
     *======================================================================*/

    /*用方式B在另一个位置显示一个像素点（绿色，右下角）*/
    lcd_put_pixel_file(var.xres/2 + 50, var.yres/2 + 50, 0x0000FF00);

    printf("方式B(lseek+write): 在屏幕偏中心位置显示一个绿点完成\n");

    /*用方式B清屏（全部像素写成黑色）*/
    {
        /*把文件指针定位到屏幕起始位置，然后写入screen_size个字节的0*/
        unsigned char *black = malloc(screen_size);
        if(black){
            memset(black, 0, screen_size);
            lseek(fd_fb, 0, SEEK_SET);  /*定位到显存起始地址（偏移量=0）*/
            write(fd_fb, black, screen_size); /*一次性写入全屏黑色像素数据*/
            free(black);
        }
    }

    printf("方式B(lseek+write): 清屏完成\n");

    /*======================================================================
     * 性能对比说明（不需要实际运行）:
     *
     * 假设要在屏幕上画一条1000像素的线：
     *
     * mmap方式:
     *   for(i=0; i<1000; i++)
     *       fb_base[offset + i] = color;   // 直接内存写，1条CPU指令，无系统调用
     *
     * lseek+write方式:
     *   for(i=0; i<1000; i++){
     *       lseek(fd, offset + i, SEEK_SET); // 系统调用，用户态→内核态切换
     *       write(fd, &color, 1);            // 系统调用，内核态→用户态切换
     *   }
     *   // 1000次lseek + 1000次write = 2000次系统调用
     *
     * 结论：大量像素操作必须用mmap；偶尔一次两次像素操作可以用lseek+write。
     *======================================================================*/

    /*取消mmap映射*/
    munmap(fb_base, screen_size);
    close(fd_fb);
    return 0;
}
