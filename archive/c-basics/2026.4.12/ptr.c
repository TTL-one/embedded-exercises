#include <stdio.h>

void test00(){
    int a,b,c,t;
    int *pa, *pb, *pc;
    printf("Enter three integers: ");
    scanf("%d %d %d", &a, &b, &c);

    pa = &a;
    pb = &b;
    pc = &c;

    if(a>b){
        t=a;
        a=b;
        b=t;
    }
    if(a>c){
        t=a;
        a=c;
        c=t;
    }
    if(b>c){
        t=b;
        b=c;
        c=t;
    }
    printf("%d<=%d<=%d\n", *pa, *pb, *pc);
    return;
}

void test01(){
    int number;
    printf("Enter an integer: ");
    scanf("%d", &number);
    int num3 = number * number * number;
    int sum = 0,begin = 0,end = 0;
    for(int i = 1;i<=num3;i+=2){
        sum = 0;
        for(int j = i;j<=num3;j+=2){
            sum += j;
            //printf("%d + ... + %d = %d\n", i, j, sum);
            if(sum == num3){
                begin = i;
                end = j;
                goto print;
            }
        }
    }
print:
    printf("%d = %d + %d + ... + %d\n", num3, begin, begin+2, end);
}

void test02(){
    int count;
    printf("请输入一个整数: ");
    scanf("%d", &count);
    int flag = 0,sum = 0 , number = 1;
    int data[count +1][4];
    for(int i = 0; i <= count; i++){
        for(int j = 0; j < 4; j++){
            data[i][j] = 0;
        }
    }
    for(int index = 3;index<=count ;index++){
        data[index][0] = index * index * index;
        flag = 0;
        number = 1;

        while(!flag && number<=data[index][0]){
            for(int i = number; i<=data[index][0]; i+=2){
                sum += i;
                if(sum == data[index][0]){
                    data[index][1] = number;
                    data[index][2] = i;
                    data[index][3] = 1;
                    flag = 1;
                    break;
                }
            }
            sum = 0;
            number += 2;
        }
    }
    printf("是否要打印所有式子(y/n): ");
    char c;
    scanf(" %c", &c);
    if(c == 'y' || c == 'Y'){
        for(int i = 3; i <= count; i ++){
            if(data[i][3] == 1){
                printf("%d^3 = %d + %d + ... + %d\n", i, data[i][1], data[i][1] + 2, data[i][2]);
            }
        }
    }
}

int main(){

    //test00();
    //test01();
    test02();
    return 0;
}