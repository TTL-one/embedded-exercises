#include <stdio.h>
#define NL '\n'
#define S(r) 3.14*(r)*(r)
#define C(r) 2*3.14*(r)
#define value(jin,yuan) (jin)*(yuan)*0.5
#define fanqie 3.7
#define jielan 7
#define xiqin 1.3
#define kongxincai 8
#define yangcong 2.4
#define youcai 9
#define huanggua 6.3
#define bailuobo 0.5
int main()
{
    printf("Line1%c",NL);
    printf("Line2%c",NL);

    double r=5;
    printf("半径为%.0f的圆，周长是%.2f，面积是%.2f\n",r,C(r),S(r));
    printf("小明需要支付%.2f元\n",value(2,fanqie)+value(1,kongxincai)+value(1,youcai));
    printf("小红需要支付%.2f元\n",value(3,xiqin)+value(0.5,yangcong)+value(5,huanggua));
    printf("小甲鱼需要支付%.2f元\n",value(10,huanggua)+value(20,bailuobo));
    return 0;
}