#include<stdio.h>

int main(){
    int a=0x12345678;
    printf("%x\n",a);
    void *p = &a;
    printf("%x\n",*(char*)p);
    p++;
    printf("%x\n",*(char*)p);
    p++;
    printf("%x\n",*(char*)p);
    p++;
    printf("%x\n",*(char*)p);
    return 0;
}