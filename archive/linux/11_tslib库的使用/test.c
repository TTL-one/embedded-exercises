#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <tslib.h>
#include <math.h>

static const char *tsdevice = "/dev/input/event0"; // 触摸屏设备路径

static const int MAX_SAMPLES = 5; // 最大触摸点数

static struct ts_sample_mt g_historySample[MAX_SAMPLES]; // 存储上一次的触摸点数据
static int g_doubleClickedId[2];

int main(int argc,char **argv){

	struct tsdev *ts;

    ts = ts_setup(tsdevice, 0);
	if (!ts) {
		perror("ts_setup");
		return errno;
	}

    struct input_absinfo slot;

	if (ioctl(ts_fd(ts), EVIOCGABS(ABS_MT_SLOT), &slot) < 0) {
		perror("ioctl EVIOGABS");
		ts_close(ts);
		return errno;
	}

    int max_slots = slot.maximum + 1 - slot.minimum;
	struct ts_sample_mt *samp_mt = NULL;

    samp_mt = calloc(max_slots,sizeof(struct ts_sample_mt));
    if (!samp_mt) {
        ts_close(ts);
        return -ENOMEM;
    }
    while(1)   { 
        //每次读取最多点数为max_slots的触摸点数据
        int ret = ts_read_mt(ts,samp_mt,max_slots,1);
        if (ret < 0) {
            perror("ts_read_mt");
            free(samp_mt);
            ts_close(ts);
            return 1;
        }

        if(ret == 2){
            //有两个点，可以判断是否是释放信息
            if(samp_mt[0].tracking_id==-1||samp_mt[1].tracking_id==-1){
                printf("有一个点释放了\n");
            }else{
            int x1 = samp_mt[0].x;
            int y1 = samp_mt[0].y;
            int x2 = samp_mt[1].x;
            int y2 = samp_mt[1].y;
            double distance = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
            printf("当前两点之间的距离是: %.2f\n", distance);
            }
        }
        //这是同时上报两个触点才可以的情况，
        //如果10s前摁下一个，现在又摁下一个，分开报的点怎么办呢？

        // 同时存在多个点的情况，slot和trackingid都是一定的 释放之后就没了
        // 所以历史记录可以使用slot和trackingid记录
        // 1.先摁一个点，不存在slot 只有trackingid 持续上报信息
        // 2.再摁一个点，存在slot和trackingid 持续上报信息 这两种情况按照以上办法处理就行
        // 3.多个点的情况，通过历史数组判断tracking是不是为-1来判断是否释放了，如果历史数组中只有两个点则打印



        //valid touch screen lib 实现的 ts_sample_mt中设计好的，表示当前槽位有没有上报新数据
        for (int i = 0; i < max_slots; i++) {
            if (samp_mt[i].valid) {  // 或者根据驱动规则
                g_historySample[i] = samp_mt[i];
            }
        }

        int count = 0;
        for(int i=0;i<MAX_SAMPLES;i++){
            if(g_historySample[i].tracking_id!=-1){
                g_doubleClickedId[count] = g_historySample[i].slot;
                count++;
            }
        }    
        if(count==2){
            int x1 = g_historySample[g_doubleClickedId[0]].x;
            int y1 = g_historySample[g_doubleClickedId[0]].y;
            int x2 = g_historySample[g_doubleClickedId[1]].x;
            int y2 = g_historySample[g_doubleClickedId[1]].y;
            double distance = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
            printf("当前两点之间的距离是: %.2f\n", distance);
        }
    }
    free(samp_mt);
    ts_close(ts);    
    return 0;
}