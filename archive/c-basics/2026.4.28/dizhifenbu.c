#include <stdio.h>
#include <stdlib.h>
void test00(){
    printf("请输入一个整数:");
    int num = 1;
    char * number = NULL;
    number = (char *)realloc(number,num);
    number[num-1]=(char)getchar();
    while(number[num-1]!='\n'){
        num++;
        number = (char *)realloc(number,num);
        number[num-1]=(char)getchar();
    }
    number = (char *)realloc(number,++num);
    number[num-1] = '\0';
    printf("你输入的整数是:%s\n",number);
    free(number);
}

int main(){

    test00();
    return 0;
}