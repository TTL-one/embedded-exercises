
#include<stdio.h>
void test00(){
    int touzie = 10000;
    double danlilv = 0.1;
    double fuhelilv = 0.05;
    int year = 0;
    double sum1 = touzie;
    double sum2 = touzie;
    while(sum1 >= sum2){
        sum1 += touzie * danlilv;
        printf("第%d年，单利总金额：%.2lf\n", year, sum1);
        sum2 += sum2 * fuhelilv;
        printf("第%d年，复利总金额：%.2lf\n", year, sum2);
        year++;
    }
    printf("年数：%d\n", year);
}

void test01(){
    double zonge = 4000000;
    double hongli = 0.08;
    int year = 0;
    while(zonge>=0){
        zonge -= 500000;
        zonge += zonge * hongli;
        year++;
    }
    printf("年数：%d\n", year);
}

void test02(){
    double pai = 0;
    int fenmu = 1;
    int fuhao = 1;
    double weixiang = 1;
    while(weixiang > 10E-8){
        weixiang = (double)1.0 / fenmu;
        pai += weixiang * fuhao;
        fuhao = -fuhao;
        fenmu += 2;
    }
    printf("pai的值为：%.10lf\n", pai*4);
}

void test03(){
    long a = 1, b = 1, c, i;

    for (i = 3; i <= 24; i++)
    {
            c = a + b;
            a = b;
            b = c;
    }

    printf("两年后，总共有%ld只兔子！\n", c);
}

int main(){
    //test00();
    //test01();
    //test02();
    test03();
    return 0;
}