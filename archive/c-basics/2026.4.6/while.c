#include <stdio.h>
#include <stdlib.h>

void test00(){
    int count = 0;
    char ch;
    while((ch = (char)getchar())!='\n'){
        if(ch>='A'&&ch<='Z'){
            count++;
        }
    }
    printf("你一共输入了%d个大写字母\n", count);
}

void test01(){
    char ch;
    printf("请输入一行字符串：");
    while((ch = (char)getchar())!='\n'){
        if(ch>='A'&&ch<='Z'){
            putchar(ch+32);
        }
        else if(ch>='a'&&ch<='z'){
            putchar(ch-32);
        }else{
            putchar(ch);
        }
    } 
    putchar('\n');
}

void test02(){
    char ch;
    int sum = 0;
    int flag = 0; // 标志位，默认没接收到数据
    printf("请输入一行字符串：");
    while((ch = (char)getchar())!='\n'){
        if(ch>='0'&&ch<='9'){
            sum = sum*10 + (ch - '0'); 
            flag = 1; // 接收到数字，设置标志位
        }else if(flag == 1){
            break;
        }
    }
    if(flag){
        if(sum>65535){
            printf("结果超出范围，结果未定义！\n");
            return;
        }
        printf("数字之和为：%d\n", sum);
    }else{
        printf("并未找到任何数值\n");
    }
}

int main(){
    //test00();
    //test01();
    test02();
    return 0;
}