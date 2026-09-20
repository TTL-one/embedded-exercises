#include <stdio.h>

#define MAX 100
int sum = 0;  // 统计解法总数

// 修复1：形参改为 int table[8][8]，兼容二维数组
// 修复2：这个函数改为【判断位置是否合法】（行、列、对角线都不冲突）
int isSafe(int table[8][8], int row, int col) {
    int i, j;
    // 1. 判断同一列（行不用判断，因为每行只放一个）
    for (i = 0; i < row; i++)
        if (table[i][col])
            return 0;
    // 2. 判断左上对角线
    for (i = row, j = col; i >= 0 && j >= 0; i--, j--)
        if (table[i][j])
            return 0;
    // 3. 判断右上对角线
    for (i = row, j = col; i >= 0 && j < 8; i--, j++)
        if (table[i][j])
            return 0;
    return 1;
}

// 修复3：用【回溯递归】代替暴力循环（八皇后标准解法，最小改动）
void solve(int table[8][8], int queenPosition[MAX][8], int row) {
    // 递归终止：8行都放完，得到一种解法
    if (row == 8) {
        sum++;
        // 打印解法
        printf("解法%d：", sum);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (table[i][j] == 1)
                    printf("%d ", j+1);
            }
        }
        printf("\n");
        return;
    }

    // 尝试当前行的每一列
    for (int col = 0; col < 8; col++) {
        // 核心判断：位置合法（你原来的if判断，只改这里！）
        if (isSafe(table, row, col)) {
            table[row][col] = 1;        // 放置皇后
            queenPosition[sum][row] = col; // 记录位置
            solve(table, queenPosition, row + 1); // 递归下一行
            table[row][col] = 0;        // 回溯！撤回皇后（你最缺的一步）
        }
    }
}

void test00() {
    int Table[8][8] = {0};       // 棋盘：0=空，1=皇后
    int queenPosition[MAX][8] = {0}; // 存储所有解法

    solve(Table, queenPosition, 0); // 从第0行开始回溯
    printf("\n八皇后总解法数：%d\n", sum);
}

int main() {
    test00();
    return 0;
}