#include <stdio.h>
#include <string.h>

void test00(){
    char *array[5]={"FishC","Five","Star","Good","WoW"};
    char *(*ptr)[5]=&array;
    int i,j;
    for(i=0;i<5;i++){
        for(j=0;array[i][j]!='\0';j++){
            printf("%c ",(*ptr)[i][j]);
        }
        printf("\n");
    }
}

void test01(){
    char *array[5]={"FishC","Five","Star","Good","WoW"};
    char **ptr=array;
    int i,j;
    int colMax=0,rowMax=sizeof(array)/sizeof(array[0]);
    for(i=0;i<rowMax;i++){
        int col=0;
        while(array[i][col++]!='\0');
        if(col>colMax){
            colMax=col;
        }
    }
    int arrLen[rowMax];
    for(i = 0;i<rowMax;i++){
        arrLen[i]=strlen(array[i]);
    }
    for(j=0;j<colMax-1;j++){
        for(i=0;i<rowMax;i++){
            if(j<arrLen[i]){
                printf("%c ",ptr[i][j]);
            }
            else{
                printf(" ");
            }
        }
        printf("\n");
    }
}

int main(){
    test00();
    //test01();
    return 0;
}