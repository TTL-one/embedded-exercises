#include <stdio.h>
#include <stdlib.h>

void test00(){
    int num;
    printf("请输入待录入整数的个数：");
    scanf("%d",&num);
    int *ptr =(int *)malloc(sizeof(int)*num);
    for(int i=0; i<num; i++){
        printf("请录入第%d个整数：",i+1);
        scanf("%d",ptr+i);
    }
    for(int i=0; i<num; i++){
        printf("%d ",*(ptr+i));
    }
    printf("\n");
    free(ptr);
}

void test01(){
    int num = 0;
    int *ptr =NULL;
    while((ptr=malloc(num++))!=NULL){
        free(ptr);
    }
    printf("最大可申请空间是:%d字节\n",num);
}

int main(){
    //test00();
    test01();
    return 0;
}