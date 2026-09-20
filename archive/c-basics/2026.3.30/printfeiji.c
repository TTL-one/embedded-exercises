#include <stdio.h>

int main(){
    //我的
    printf("         @         \n");
    printf("        / \\        \n");
    printf("        * *        \n");
    printf("        * *        \n");
    printf("        * *        \n");
    printf("    * * * * * *    \n");
    printf("  * * * * * * * *  \n");
    printf("* * * * * * * * * *\n");
    printf("        * *        \n");
    printf("        * *        \n");
    printf("      * * * *      \n");
    printf("    * * * * * *    \n");
    //\就是写代码换行的作用，把下一行代码和当前这一行视作同一行
    //小甲鱼 写成一行其实就是 然后用\换行书写
    printf("\n\
             @\n\
            / \\\n\
            * *\n\
            * *\n\
            * *\n\
        * * * * * *\n\
    * * * * * * * *\n\
    * * * * * * * * * *\n\
            * *\n\
            * *\n\
        * * * *\n\
        * * * * * *\n");
    return 0;
}