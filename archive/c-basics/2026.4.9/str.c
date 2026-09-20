#include <stdio.h>

void test00(){
    char str1[100], str2[100];
    printf("请输入第一个字符串：");
    scanf("%s", str1);
    printf("请输入第二个字符串：");
    scanf("%s", str2);
    int number;
    printf("请输入要比较的字符数：");
    scanf("%d", &number);
    number--;
    while(number&& str1[number] && str2[number] && str1[number] == str2[number]){
        number--;
    }
    if(number == 0){
        printf("0");
    }else if(str1[number] > str2[number]){
        printf("%d", (int)str1[number]-(int)str2[number]);
    }else{
        printf("%d", (int)str2[number]-(int)str1[number]);
    }
}

void test01(){
    char str[21];
    printf("请输入一个字符串：");
    int index = 0;
    int lastspaceindex = -1;
    while(scanf("%c",&str[index]) && str[index] != '\n' && index < 20){
        if(str[index] == ' '){
            lastspaceindex = index;
        }
        index++;
    }
    //如果不超长，这个循环进不去
    while(str[index] != '\n' && scanf("%c",&str[index]) ){
        // 超出长度的一直在str[20]的位置输入，丢弃
    }
    if(lastspaceindex != -1){
        index = lastspaceindex + 1;
    }
    str[index] = '\0';
    printf("您输入的字符串是：%s\n", str);
}

void test02(){
    char str[100];
    printf("请输入一个字符串：");
    int index = 0;
    int count[96] = {0};
    while(scanf("%c",&str[index]) && str[index] != '\n'){
        count[(int)str[index]]++;
        index++;
    }
    printf("您输入的字符串一共有：%d个字符\n", index);
    printf("各字符出现次数：\n");
    for(int i = 0; i < 96; i++){
        if(count[i] > 0){
            printf("%c: %d\n", (char)(i), count[i]);
        }
    }
    printf("其中出现次数最多的字母是：");
    int max = 0;
    char max_char = 'a';
    for(int i = 0; i < 96; i++){
        if(count[i] > max){
            max = count[i];
            max_char = (char)(i);
        }
    }
    printf("%c\n", max_char);
}

int main(){
    //test00();
    //test01();
    test02();
    return 0;
}