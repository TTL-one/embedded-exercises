#include <stdio.h>
#include <stdbool.h>
void test00() {
    for(int i = 1; i<21; i++){
        printf("%d - %d\n", i, 23+(i-1)*14);
    }
}

void test01(){
    char ch = '0';
    int sum = 0;
    while(1){
        printf("请输入一个合法的字符：");
        scanf(" %c", &ch);  
        if(ch>='0'&&ch<='9'){
          
            sum = sum + ch-'0';
        }else{
            break;
        }
    }
    printf("数字字符的和为：%d\n", sum);
}

bool bSumOneLine(double * result){
    char ch = '0';
    double sum = 0;
    double xiaoshu = 0;
    int flag = 0; // 标志位，默认没接收到数据
    printf("请输入一个合法的字符：");
    while((ch = (char)getchar())!='\n'){
        if(!flag && (ch<'0'||ch>'9')){
            return false; //退出循环，结束输入
        }
        if((ch<'0'||ch>'9')&&(ch!='.')&&flag){
            xiaoshu = 0;
            continue;
        }
        if(!xiaoshu){
            if(ch>='0'&&ch<='9'){  
                sum = sum + ch-'0';
                flag = 1; // 接收到数字，设置标志位
            }else if(ch=='.'){
                xiaoshu = 1; // 遇到小数点，开始处理小数部分
            }
        }else{
            if(ch>='0'&&ch<='9'){
                xiaoshu *= 0.1;  
                sum = sum + (ch-'0')*xiaoshu;
            }else{
                break;
            }
        }
    }
    //printf("数字字符的和为：%.2f\n", sum);
    *result = sum;
    return true;   
}

void test02(){
    double sum = 0 ;
    double result;
    while(bSumOneLine(&result)){
        sum += result;
    }
    printf("数字字符的和为：%.2f\n", sum);
}

void test03(){
    float num = 0;
    double sum = 0;
    int status;
    /*
    1. scanf("%f") 的核心行为
    自动跳过所有空白字符（空格、\n、Tab），直到遇到第一个非空白字符。
    若字符是数字、+、-、.，则读取完整数字，直到遇到第一个非数字字符停止，把这个非数字字符留在输入缓冲区。
    读取成功返回1，失败返回0，字符留在缓冲区。
    2. getchar() 的核心行为
    从缓冲区读取一个字符（不管是字母、空格、换行、符号），读完直接从缓冲区删除。
    缓冲区为空时，阻塞等待用户输入。
    */
    // scanf一直拿的是数字，getchar一直拿的是非数字字符（包括换行符）
    do
    {
            printf("请输入合法的数字：");
            do
            {
                    sum = sum + num;
                    status = scanf("%f", &num);
            } while (getchar() != '\n' && status == 1);
    } while(status == 1);
    printf("结果是：%.2lf\n", sum);
    return 0;
}

int main() {
    //test00();
    //test01();
    //test02();
    test03();
    return 0;
}