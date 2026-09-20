#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// 检查数字是否不在数组中（洗牌备用）
int findNumberNotInArray(int num, int *array, int size) {
    for (int i = 0; i < size; i++) {
        if (array[i] == num) {
            return 0;
        }
    }
    return 1;
}

// 【修复】牌面显示函数：使用静态缓冲区，正确匹配花色+数字
char *showPai(int num) {
    // 静态数组，保证返回的字符串有效
    static char str[20];
    char JQK[] = {'J', 'Q', 'K'};
    
    // 大小王
    if (num == 54) {
        strcpy(str, "大王");
        return str;
    }
    if (num == 53) {
        strcpy(str, "小王");
        return str;
    }

    // 标准扑克规则：1-13方块 14-26梅花 27-39红桃 40-52黑桃
    if (num >= 1 && num <= 13) {
        // 方块
        int n = num;
        if (n >= 1 && n <= 10) sprintf(str, "方%d", n);
        else sprintf(str, "方%c", JQK[n - 11]);
    } else if (num >= 14 && num <= 26) {
        // 梅花
        int n = num - 13;
        if (n >= 1 && n <= 10) sprintf(str, "梅%d", n);
        else sprintf(str, "梅%c", JQK[n - 11]);
    } else if (num >= 27 && num <= 39) {
        // 红桃
        int n = num - 26;
        if (n >= 1 && n <= 10) sprintf(str, "红%d", n);
        else sprintf(str, "红%c", JQK[n - 11]);
    } else if (num >= 40 && num <= 52) {
        // 黑桃
        int n = num - 39;
        if (n >= 1 && n <= 10) sprintf(str, "黑%d", n);
        else sprintf(str, "黑%c", JQK[n - 11]);
    } else {
        strcpy(str, "无效牌");
    }
    return str;
}

// 【修复】Fisher-Yates 经典洗牌算法（无重复、效率高）
void shuffle(int *pai, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // 交换
        int temp = pai[i];
        pai[i] = pai[j];
        pai[j] = temp;
    }
}

void xipaiTask() {
    // 【修复】玩家名字：定义固定大小数组，避免内存错误
    char player[3][20];
    printf("请输入1号玩家的名字：");
    scanf("%s", player[0]);
    printf("请输入2号玩家的名字：");
    scanf("%s", player[1]);
    printf("请输入3号玩家的名字：");
    scanf("%s", player[2]);

    // 初始化 1~54 扑克牌
    int pai[54];
    for (int i = 0; i < 54; i++) {
        pai[i] = i + 1;
    }

    char chongxing = 'Y';
    while (chongxing == 'Y' || chongxing == 'y') {
        // 洗牌
        shuffle(pai, 54);
        printf("\n===== 洗牌完成 =====\n");
        printf("方=方块，梅=梅花，红=红桃，黑=黑桃\n\n");

        // 【修复】正确发牌：每人18张
        // 玩家1：0-17  玩家2：18-35  玩家3：36-53
        for (int i = 0; i < 3; i++) {
            printf("%s 的牌：", player[i]);
            int start = i * 18;
            for (int j = 0; j < 18; j++) {
                printf("%s ", showPai(pai[start + j]));
            }
            printf("\n");
        }

        // 【修复】吸收缓冲区 + 正确取地址
        getchar();
        printf("\n重新洗牌(Y/N)?：");
        scanf("%c", &chongxing);
    }
    printf("程序退出！\n");
}

int main() {
    // 初始化随机数种子
    srand((unsigned)time(NULL));
    // 调用洗牌发牌函数
    xipaiTask();
    return 0;
}