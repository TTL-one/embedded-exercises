#include <stdio.h>

//采用回溯法试错，暴力拆解问题
//先实现是否能在这里放置马的问题
static int g_Table[8][8] = {0};
int count = 1; // 标记步数，方便打印查看（你没加，我加个小辅助）

int couldbeSet(int row,int col){
    if(row<0||row>7){
        return 0;
    }
    if(col<0||col>7){
        return 0;
    }    
    if(!g_Table[row][col]){
        return 1;
    }else{
        return 0;
    }
}

int tableIsFull(){
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(!g_Table[i][j]){
                return 0;
            }
        }
    }
    return 1;
}

//放一个位置 
//然后尝试下一步位置 
//成功就接着放 否则返回上一步尝试另一个位置
//直到整个棋盘都布满
void setPosition(int row,int col)
{
    //先写递归结束条件 整个棋盘都布满
    if(tableIsFull()){
        // 修复1：走满了打印棋盘！
        printf("马踏棋盘成功！\n");
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                printf("%2d ",g_Table[i][j]);
            }
            printf("\n");
        }
        return;
    }
    int position[8][2] ={
        {row - 2, col -1},
        {row - 2, col +1},
        {row - 1, col +2},
        {row + 1, col +2},
        {row + 2, col +1},
        {row + 2, col -1},
        {row + 1, col -2},
        {row - 1, col -2}
    };
    for(int i =0; i<8; i++){
        int r = position[i][0];
        int c = position[i][1];
        if(couldbeSet(r,c)){
            g_Table[r][c] = ++count; // 修复2：用步数标记，更直观
            setPosition(r,c);
            // 回溯
            g_Table[r][c] = 0;
            count--;
        }
    }
}

// 修复3：补全main函数，设置起点！
int main(){
    // 从 左上角(0,0) 开始，第一步标记为1
    g_Table[0][0] = 1;
    // 启动递归
    setPosition(0, 0);
    return 0;
}