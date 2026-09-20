#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 胜负判断函数（修复逻辑+语法错误）
int win(int num1,int num2){
    // 平局
    if(num1 == num2){
        return 1;
    }
    // 赢的规则：剪刀(1)赢布(3)，石头(2)赢剪刀(1)，布(3)赢石头(2)
    if( (num1 == 1 && num2 == 3) || (num1 == 2 && num2 == 1) || (num1 == 3 && num2 == 2) ){
        return 2; // 我赢了
    }else{
        return 0; // 你赢了
    }
}

void test00(){
    // srand 只写1次！！！
    srand((unsigned int)time(NULL));
    
    char *jihe[3]={
        "剪刀",
        "石头",
        "布"
    };
    char *jieguo[3]={
        "你赢了",
        "咱打平",
        "我赢了"
    };
    
    int *jilu;
    int num = 1;
    // 动态内存分配（规范写法）
    jilu = (int *)malloc(num * sizeof(int));
    if(jilu == NULL){
        printf("内存分配失败！");
        return;
    }

    while(1){ // 统一用循环，代码更简洁
        printf("请出拳(1剪刀/2石头/3布/0退出)->");
        scanf("%d",&jilu[num-1]);
        
        // 输入0，直接退出游戏
        if(jilu[num-1] == 0){
            break;
        }
        // 输入非法数字判断
        if(jilu[num-1] <1 || jilu[num-1]>3){
            printf("输入错误！请输入1-3\n");
            continue;
        }

        // 电脑随机出拳（每次循环生成新的）
        int wode=rand()%3+1;
        // 打印结果
        printf("你出%s,我出%s → %s\n",jihe[jilu[num-1]-1],jihe[wode-1],jieguo[win(jilu[num-1],wode)]);

        // 扩容记录数组（规范realloc）
        num++;
        int *temp = (int *)realloc(jilu, num * sizeof(int));
        if(temp != NULL){
            jilu = temp;
        }else{
            printf("扩容失败！");
            break;
        }
    }

    // 释放内存
    free(jilu);
    jilu = NULL;
    printf("\n游戏结束！\n");
}

int main(){
    test00();
    return 0;
}