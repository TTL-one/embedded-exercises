#include <stdio.h>

void test00(){
    char *string=NULL;
    printf("请输入一个英文句子：");
    scanf("%s",&string);
    int index=0,sumSpace=0,maxWord=-1,nowWord=0;
    while(string[index])
    {
        if(string[index]==' '&&string[index+1]!=' '){
            sumSpace++;
            maxWord=nowWord>maxWord?nowWord:maxWord;
            nowWord = 0;
        }
        index++;
        nowWord++;
    }
    char result[sumSpace+1][maxWord+1];
    index = 0;
    for(int i = 0;i<sumSpace+1;i++){
        int j = 0;
        for(;j<maxWord;j++){
            result[i][j]=string[index++];
        }
        result[i][j]='\0';
        index++;
    }
    printf("分割结果已存放到result[%d][%d]的二维数组中...\n",sumSpace+1,maxWord+1);
    printf("现在依次打印每个单词：\n");
    for(int i =0;i<sumSpace+1;i++){
        printf("%s\n",result[i]);
    }

}

void test01() {
    // 1. 定义固定大小数组接收输入（新手必用，无野指针）
    char str[1000];
    printf("请输入一个英文句子：");
    // 读取带空格的整行字符串（核心：%[^\n] 读到换行符为止）
    scanf("%[^\n]", str);

    int i = 0;
    int word_num = 0;   // 单词总数
    int max_len = 0;    // 最长单词长度
    int current = 0;    // 当前单词长度

    // ============= 第一次遍历：统计 单词数量 + 最长单词长度 =============
    while (1) {
        // 读到字符串结束，退出循环
        if (str[i] == '\0') {
            // 处理最后一个单词
            if (current > 0) {
                word_num++;
                if (current > max_len) max_len = current;
            }
            break;
        }
        // 判断是否是空格
        if (str[i] == ' ') {
            // 前一个是字符，说明一个单词结束
            if (current > 0) {
                word_num++;
                if (current > max_len) max_len = current;
                current = 0;
            }
        } else {
            // 非空格，单词长度+1
            current++;
        }
        i++;
    }

    // 无单词的情况
    if (word_num == 0) {
        printf("未输入单词！\n");
        return;
    }

    // ============= 变长数组！严格满足题目省空间要求 =============
    char result[word_num][max_len + 1];
    int row = 0;   // 二维数组行号（第几个单词）
    int col = 0;   // 二维数组列号（单词第几个字符）
    i = 0;

    // ============= 第二次遍历：分割单词，存入变长二维数组 =============
    while (1) {
        if (str[i] == '\0') {
            // 给最后一个单词加结束符
            result[row][col] = '\0';
            break;
        }
        if (str[i] == ' ') {
            if (col > 0) {
                // 单词结束，添加结束符
                result[row][col] = '\0';
                row++;
                col = 0;
            }
        } else {
            // 存入字符
            result[row][col] = str[i];
            col++;
        }
        i++;
    }

    // ============= 打印结果 =============
    printf("分割结果已存放到 result[%d][%d] 二维数组中...\n", word_num, max_len + 1);
    printf("依次打印每个单词：\n");
    for (int k = 0; k < word_num; k++) {
        printf("%s\n", result[k]);
    }
}


int main(){
    test00();
    return 0;
}