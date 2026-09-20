#include <stdio.h>

// 请在此处补充一个宏定义，让程序可以统计 COUNTS 参数的个数
#define ARGUMENTS(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,N,...) N
#define COUNTS(...) ARGUMENTS(FISHC, ## __VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define PRINT_COUNT(...) printf("COUNTS(%s)有 %d 个参数。\n", #__VA_ARGS__,COUNTS(__VA_ARGS__))
int main(void)
{
        printf("COUNTS()有 %d 个参数。\n", COUNTS());
        printf("COUNTS(1, 2, 3)有 %d 个参数\n", COUNTS(1, 2, 3));
        //printf("COUNTS("Apple", "Banana", "Cat", "Dog")有 %d 个参数\n", COUNTS("Apple", "Banana", "Cat", "Dog"));

        return 0;
}