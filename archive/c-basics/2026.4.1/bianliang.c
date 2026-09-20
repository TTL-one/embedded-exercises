#include <stdio.h>

int main(){
    int a,b;
    b=3;
    a=b;
    b=b*2;
    printf("a = %d\n",a);
    printf("b = %d\n",b);

    double r,c,s;
    r=5;
    c=2*3.14*r;
    s=3.14*r*r;
    printf("半径为%.0f的圆，周长是%.2f，面积是%.2f\n",r,c,s);

    printf("打印五个字符：%c %c %c %c %c\n",'F','i','s','h','C');
    printf("前面用 填充：% 10d\n",2015);
    printf("前面用0填充：%010d\n",2015);
    printf("右对齐，保留两位：% 10.2f\n",3.1416);
    printf("左对齐，保留两位：%-10.3f\n",3.1416);
    printf("右对齐，指数形式：%.6e\n",520000.0);
    printf("左对齐，指数形式：%-.6E\n",520000.0);
    printf("右对齐，指数形式：%10e\n",520000.0);
    printf("左对齐，指数形式：%-10E\n",520000.0);
}