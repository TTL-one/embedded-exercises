#include <stdio.h>
#include <math.h>
void test00(){
    int a = 0;
    int e = 0;
    int i = 0;
    int o = 0;
    int u = 0;
    int sum = 0;
    char c;
    printf("Enter a string: ");
    while((c=getchar()) != '\n'){
        switch(c){
            case 'a':
                a++;
                break;
            case 'e':
                e++;
                break;
            case 'i':
                i++;
                break;
            case 'o':
                o++;
                break;
            case 'u':
                u++;
                break;
        }
    }
    sum = a + e + i + o + u;
    printf("Total number of vowels: %d\n", sum);
    printf("Number of a's: %d\n", a);
    printf("Number of e's: %d\n", e);
    printf("Number of i's: %d\n", i);
    printf("Number of o's: %d\n", o);
    printf("Number of u's: %d\n", u);
}

void test01(){
    int sum = 0;
    int flag = 0;
    for(int i = 2; i < 10000; i++){
        for(int j = 2; j < i; j++){
            if(i%j == 0){
                flag = 1;
                break;
            }
        }
        if(!flag){
            sum++;
        }
        flag = 0;
    }
    printf("10000以内的素数: %d\n", sum);
}

void test02(){
    int sum = 0;
    int flag = 0;
    for(int i = 2; i < 10000; i++){
        for(int j = 2; j < (int)sqrt((double)i)+1; j++){
            if(i%j == 0){
                flag = 1;
                break;
            }
        }
        if(!flag){
            sum++;
        }
        flag = 0;
    }
    printf("10000以内的素数: %d\n", sum);
}

void test03(){
    char c;
    printf("Enter a string: ");
    while((c=getchar()) != '\n'){
        if(c>='a' && c<='w'){
            c=c+3;
        }
        else if(c>='A' && c<='W'){
            c=c+3;
        }
        else if(c>='x' && c<='z'){
            c=c-'x'+'a';
        }
        else if(c>='X' && c<='Z'){
            c=c-'X'+'A';
        }
        printf("%c", c);
    }
}
/*
……
for (i = 0; i < 100; i++)
{
        if (i % 2)
        {
                continue;
        }
}
……
for不会跳过i++ i++不属于循环体内的
……
i = 0;

while (i < 100)
{
        if (i % 2)
        {
                continue;
        }

        i++;
}
……
而while会跳过i++ i++属于循环体内的
*/
int main(){
    //test00();
    //test01();
    //test02();
    test03();
    return 0;
}