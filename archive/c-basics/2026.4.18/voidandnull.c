#include <stdio.h>

int main(){
    int M,P,N;
    printf("请输入第一个矩阵的尺寸(M*P)：");
    scanf("%d*%d", &M, &P);
    printf("请输入第二个矩阵的尺寸(P*N)：");
    scanf("%d*%d", &P, &N);
    int matrixA[M][P];
    int matrixB[P][N];
    printf("请输入第一个矩阵的元素：\n");
    for(int i=0;i<M;i++){
        for(int j=0;j<P;j++){
            scanf("%d", &matrixA[i][j]);
        }
    }
    printf("请输入第二个矩阵的元素：\n");
    for(int i=0;i<P;i++){
        for(int j=0;j<N;j++){
            scanf("%d", &matrixB[i][j]);
        }
    }
    int matrixC[M][N];
    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){
            matrixC[i][j] = 0;
            for(int k=0;k<P;k++){
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
    int row = (M>N)?M:N;
    printf("计算结果如下：\n");
    for(int i = 0;i<row;i++){
        printf("|  ");
        for(int j = 0; j<P;j++){
            if(i < M){
                printf("\b%d |", matrixA[i][j]);
            }
            else{
                printf("\b\b\b  ");
            }
        }
        if( i == row/2){
            printf(" * |  ");
        }
        else{        
            printf("   |  ");
        }
        for(int j = 0; j<N;j++){
            if(i < P){
                printf("\b%d |", matrixB[i][j]);
            }
            else{
                printf("\b\b\b  ");
            }
        }
        if( i == row/2){
            printf(" =  |  ");
        }
        else{        
            printf("    |  ");
        }
        for(int j = 0; j<N;j++){
            if(i<M){
                printf("\b%d |", matrixB[i][j]);
            }else{
                printf("\b\b\b   ");
            }
        }
        printf("\n");
    }
    return 0;
}