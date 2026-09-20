#include <stdio.h>
#include <math.h>
void test01(){
    int count = 20;
    char a[count]; // Variable Length Array (VLA) with size 9
    int i = 0;
    printf("请输入字符：");
    while(scanf("%c", &a[i]) != EOF && a[i] != '\n'){
        i++;
    }
    int size = sqrt(i); 
    char (*b)[size] = (char (*)[size])a;
    for(int j=0;j<size;j++){
        for(int k=0;k<size;k++){
            printf("%c ",b[j][k]);
        }
        printf("\n");
    }
}

void test02(){
    int start = 2014;
    float a[3][12]={
        {-1,-1,-1,-1,-1,-1,-1,31.3,35.5,58.7,49.6,55.5},
        {59.8,54.9,33.1,38.2,26.6,20.5,27.8,38.5,41.5,44.7,38.1,41.5},
        {34.9,36.4,47.5,37.9,30.6,23.4,26.6,34.3,-1,-1,-1,-1}
    };
    int yearmonth[2]={0,0};
    printf("请输入年月（格式：年-月）：");
    scanf("%d-%d",&yearmonth[0],&yearmonth[1]);
    if(yearmonth[0]<start || yearmonth[0]>start+2 || yearmonth[1]<1 || yearmonth[1]>12){
        printf("输入的年月不合法！\n");
        return;
    }
    if(a[yearmonth[0]-start][yearmonth[1]-1] == -1){
        printf("%d年%d月广州的PM2.5值缺失！\n",yearmonth[0],yearmonth[1]);
        return;
    }
    printf("%d年%d月广州的PM2.5值是：%.2f\n",yearmonth[0],yearmonth[1],a[yearmonth[0]-start][yearmonth[1]-1]);
}

void test03(){
    int start = 2014;
    float a[3][12]={
        {-1,-1,-1,-1,-1,-1,-1,31.3,35.5,58.7,49.6,55.5},
        {59.8,54.9,33.1,38.2,26.6,20.5,27.8,38.5,41.5,44.7,38.1,41.5},
        {34.9,36.4,47.5,37.9,30.6,23.4,26.6,34.3,-1,-1,-1,-1}
    };
    int maxmin[2]={0,0};
    for(int i=0;i<3;i++){
        for(int j=0;j<12;j++){
            if(a[i][j] != -1){
                if(maxmin[0] == 0 || a[i][j] > a[maxmin[0]/12][maxmin[0]%12]){
                    maxmin[0] = i*12 + j;
                }
                if(maxmin[1] == 0 || a[i][j] < a[maxmin[1]/12][maxmin[1]%12]){
                    maxmin[1] = i*12 + j;
                }
            }
        }
    }
    printf("最大值在第%d个位置，最小值在第%d个位置\n", maxmin[0], maxmin[1]);
    printf("最大值是：%.2f，最小值是：%.2f\n", a[maxmin[0]/12][maxmin[0]%12], a[maxmin[1]/12][maxmin[1]%12]);
    int  guiyi[3][12];
    for(int i=0;i<3;i++){
        for(int j=0;j<12;j++){
            if(a[i][j] != -1){
                guiyi[i][j] = (a[i][j] - a[maxmin[1]/12][maxmin[1]%12])+1;
            }else{
                guiyi[i][j] = -1;
            }
        }
    }
    for(int i=0;i<3;i++){
        for(int j=0;j<12;j++){
            if(guiyi[i][j] != -1){
                printf("%d年%d月：", start+i, j+1);
                for(int k = 0;k<guiyi[i][j];k++){
                    printf("*");
                }
                printf("\n");
            }else{
                continue;
            }
        }
    }
}
int main(){
    //test00();
    //test02();
    test03();
    return 0;
}