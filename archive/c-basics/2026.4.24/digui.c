#include <stdio.h>
void test00(){
    char string[100]={0};
    int index =0;
    while(scanf("%c",&string[index])&&string[index++]!='!');
    index--;
    printf("字符串长度：%d\n",index);

    printf("逆序结果：");
    for(int i = index; i>0; i--){
        printf("%c",string[i-1]); 
    }
    printf("\n");
}

int fibo(int n){
    if(n==1){
        return 1;
    }else if (n == 2){
        return 1;
    }else{
        return fibo(n-1)+fibo(n-2);
    }
}


void test01(){
    printf("%d",fibo(10));
}

void decToBin(int num) {
    // 递归终止条件：数字小于等于1时，直接打印
    if (num > 1) {
        decToBin(num / 2); // 1. 先递归计算商（高位）
    }
    printf("%d", num % 2); // 2. 后打印余数（低位），实现逆序
}

void test02(){

}

int main(){
    //test00();
    //test01();
    test02();
    return 0;
}