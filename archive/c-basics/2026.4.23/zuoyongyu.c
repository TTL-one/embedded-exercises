#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h> 
// 1. 检查数组是否有序（猴子排序必备）
bool is_sorted(int array[], int length) {
    for (int i = 0; i < length - 1; i++) {
        // 如果前一个数 > 后一个数，说明无序
        if (array[i] > array[i + 1]) {
            return false;
        }
    }
    return true;
}

// 2. 正确的随机洗牌算法（Fisher-Yates洗牌，替代你错误的交换）
void shuffle(int array[], int length) {
    int randindex[length];
    for(int i = 0; i<length; i++){
        randindex[i] = rand()%length ;
    }
    for(int i = 0;i<length;i++){
        int temp = array[randindex[i]];
        array[randindex[i]] = array[i];
        array[i] = temp;
    }
    printf("排序后的结果是:");
    for (int i = 0; i < length; i++)
    {
            printf("%d ", array[i]);
    }
    putchar('\n');

}
void bogo_sort(int array[],int length){
    // 核心：无限循环，直到数组有序
    while (!is_sorted(array, length)) {
        shuffle(array, length);  // 随机打乱
    }
}

int main(void)
{
        int array[] = {73, 108, 111, 118, 101, 70, 105, 104, 67};
        int i, length;
        time_t begin, end;

        begin = time(NULL);

        length = sizeof(array) / sizeof(array[0]);
        bogo_sort(array, length);

        printf("排序后的结果是：");
        for (i = 0; i < length; i++)
        {
                printf("%d ", array[i]);
        }
        putchar('\n');

        end = time(NULL);
        printf("总共耗时：%ld秒\n", end - begin);

        return 0;
}