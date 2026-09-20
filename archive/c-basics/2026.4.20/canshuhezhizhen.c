#include <stdio.h>
#include <stdarg.h>

// 你写的 itoa 函数（保留不动）
char *myitoa(int num, char *str) {
    int i = 0;
    int is_negative = 0;
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }
    if (num == 0) {
        str[i++] = '0';
    }
    while (num > 0) {
        str[i++] = num % 10 + '0';
        num /= 10;
    }
    if (is_negative) {
        str[i++] = '-';
    }
    str[i] = '\0';
    int len = i;
    for (int j = 0; j < len / 2; j++) {
        char temp = str[j];
        str[j] = str[len - 1 - j];
        str[len - 1 - j] = temp;
    }
    return str;
}
char *myitoaa(int num, char *str){
    int dec = 1;
    int i = 0;
    int number = num;
    if(num<0){
        str[i++] = '-';
        num = -num;
    }
    while(number>9){
        dec*=10;
        number/=10;
    }
    while (dec) {
        str[i++] = num / dec + '0';
        num %=dec;
        dec /=10;
    }  
    str[i] = '\0';
    return str; 

}

// ============= 修复完成的 myprintf =============
int myprintf(const char *format, ...) {
    va_list val;
    va_start(val, format);  // 绑定可变参数

    int total_len = 0;      // 总字符数
    char buf[20];           // 存数字转字符串的缓冲区
    int i = 0;

    // 遍历格式化字符串
    while (format[i] != '\0') {
        // 遇到格式符
        if (format[i] == '%') {
            i++; // 跳过 %

            // 处理 %d 整数
            if (format[i] == 'd') {
                int num = va_arg(val, int);
                myitoa(num, buf); // 数字转字符串
                // 输出字符串
                for (int j = 0; buf[j] != '\0'; j++) {
                    putchar(buf[j]);
                    total_len++;
                }
                i++; // 跳过 d
                continue;
            }

            // 处理 %c 字符
            if (format[i] == 'c') {
                char c = (char)va_arg(val, int);
                putchar(c);
                total_len++;
                i++; // 跳过 c
                continue;
            }

            // 处理 %s 字符串
            if (format[i] == 's') {
                char *s = va_arg(val, char*);
                while (*s != '\0') {
                    putchar(*s);
                    total_len++;
                    s++;
                }
                i++; // 跳过 s
                continue;
            }
        }

        // 普通字符，直接打印
        putchar(format[i]);
        total_len++;
        i++;
    }

    va_end(val);
    return total_len;
}

// 你的主函数（完全不动）
int main(void) {
    int i;

    i = myprintf("Hello %s\n", "FishC");
    myprintf("共打印了%d个字符(包含\\n)\n", i);
    i = myprintf("int: %d, char: %c\n", -520, 'H');
    myprintf("共打印了%d个字符(包含\\n)\n", i);

    return 0;
}