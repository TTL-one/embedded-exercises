#include <stdio.h>

#define MAX 1024

void test00(){
    char str[MAX];
    char *target = str;
    int length = 0;

    printf("请输入一个字符串：");
    fgets(str, MAX, stdin);

    while (*target++ != '\0')
    {
            length++;
    }

    printf("您总共输入了 %d 个字符！\n", length - 1);
}

int main()
{
    test00();
    //test01();
    return 0;
}