#include <stdio.h>
#include <math.h>
int main(){

    char i;
    unsigned char j;
    i = 255;
    j = 255;
    printf("%d\n", i);
    printf("%d\n", j);

    int number;
    printf("请输入一个整数：\n");
    scanf("%d", &number);

    printf("%d的五次方是%lld：\n",number,pow((double)number,5));
    return 0;
}