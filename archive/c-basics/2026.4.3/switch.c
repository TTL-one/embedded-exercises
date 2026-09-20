#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define GPIO8 8
#define GPIO9 9
#define GPIO10 10
#define GPIO11 11

void calc(){
            float op1, op2;
        char ch;
        float result;
        bool invalid = false;

        printf("请输入式子：");
        //记住scanf和printf使用形式其实是一样的，都是格式字符串加参数列表
        //scanf也可以读取多个变量
        //A. scanf 函数要注意的是双引号里边的内容是指定接收的格式化占位符，
        //如果在里边写上空格，那么表示该位置将读取并忽略连续的空白字符（空格、回车和制表符）
        scanf("%f %c %f", &op1, &ch, &op2);

        switch (ch)
        {
                case '-': op2 = -op2; // no break
                case '+': result = op1 + op2; break;
                case '*': result = op1 * op2; break;
                case '/':
                        if (op2 != 0)
                        {
                                result = op1 / op2;
                                break;
                        }
                        else
                        {
                                invalid = true;
                                break;
                        }
        }

        if (invalid)
        {
                printf("除数不能为零！\n");
        }
        else
        {
                printf("结果是：%.2f\n", result);
        }

        return 0;
}

typedef enum{
    LOW = 0,
    HIGH = 1
} TTL;

void vdigitalWrite(int pin, TTL dian){
    printf("Pin %d is set to %d\n", pin, dian);
}

void vqianjin(){
    vdigitalWrite(GPIO8, HIGH);
    vdigitalWrite(GPIO9, LOW);
    vdigitalWrite(GPIO10, HIGH);
    vdigitalWrite(GPIO11, LOW);
}
void vhoutui(){
    vdigitalWrite(GPIO8, LOW);
    vdigitalWrite(GPIO9, HIGH);
    vdigitalWrite(GPIO10, LOW);
    vdigitalWrite(GPIO11, HIGH);
}

void vzuoxuan(){
    vdigitalWrite(GPIO8, HIGH);
    vdigitalWrite(GPIO9, LOW);
    vdigitalWrite(GPIO10, LOW);
    vdigitalWrite(GPIO11, HIGH);
}

void vyouxuan(){
    vdigitalWrite(GPIO8, LOW);
    vdigitalWrite(GPIO9, HIGH);
    vdigitalWrite(GPIO10, HIGH);
    vdigitalWrite(GPIO11, LOW);
}

void vreset(){
    vdigitalWrite(GPIO8, LOW);
    vdigitalWrite(GPIO9, LOW);
    vdigitalWrite(GPIO10, LOW);
    vdigitalWrite(GPIO11, LOW);
}

void loop(){
    char command;
    switch (command)
    {
    case 'g':
        vqianjin();
        break;
    case 'b':
        vhoutui();
        break;
    case 'l':
        vzuoxuan();
        break;
    case 'r':
        vyouxuan();
        break;
    case 's':
        vreset();
        break;

    default:
        break;
    }
}

int main(){
    vreset();
    loop();
    return 0;
}