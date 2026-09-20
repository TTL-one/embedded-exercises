#include <stdio.h>

float add(float num1,float num2){
    return num1 + num2;
}
float sub(float num1,float num2){
    return num1 - num2;
}
float mul(float num1,float num2){
    return num1 * num2;
}
float divi(float num1,float num2){
    if(num2 == 0){
        return -1;
    }
    return num1 / num2;
}

typedef float (*op) (float,float);
float cal(op operator,float num1,float num2){
    return operator(num1,num2);
}

int main(){
    printf("请输入两个:");
    float num1,num2;
    scanf("%f %f",&num1,&num2);
    printf("对这两个数进行加减乘除后的结果是:%.2f %.2f %.2f %.2f\n",cal(add,num1,num2),cal(sub,num1,num2),cal(mul,num1,num2),cal(divi,num1,num2));
    return 0;
}