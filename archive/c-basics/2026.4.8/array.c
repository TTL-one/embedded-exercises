#include <stdio.h>

void test00(){
    int month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    printf("请输入一个年份：");
    int year;
    scanf("%d", &year);
    if((year % 4 == 0 && year % 100 != 0) || year % 400 == 0){
        month[1] = 29;
    }
    for(int i = 0; i < 12; i++){
        printf("%d ", month[i]);
    }
}

void test01(){
    printf("请输入你的生日（如：1988-05-20）：");
    int birth_year, birth_month, birth_day;
    scanf("%d-%d-%d", &birth_year, &birth_month, &birth_day);
    printf("请输入今年日期（如：2023-05-20）：");
    int current_year, current_month, current_day;
    scanf("%d-%d-%d", &current_year, &current_month, &current_day);
    int live_days = 0;
    int month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if((birth_year % 4 == 0 && birth_year % 100 != 0) || birth_year % 400 == 0){
        month[1] = 29;
    }
    live_days += month[birth_month-1] - birth_day;
    for(int i = birth_month; i < 12; i++){
        live_days += month[i];
    }
    for(int i = birth_year+1; i<=current_year-1;i++){
        if((i % 4 == 0 && i % 100 != 0) || i % 400 == 0){
            live_days += 366;
        }else{
            live_days += 365;
        }
    }
    if((current_year % 4 == 0 && current_year % 100 != 0) || current_year % 400 == 0){
        month[1] = 29;
    }else{
        month[1] = 28;
    }   
    for(int i = 0; i < current_month-1; i++){
        live_days += month[i];
    }
    live_days += current_day;
    printf("你已经活了 %d 天。\n", live_days);
}

void test02(){
    printf("请输入你的生日（如：1988-05-20）：");
    int birth_year, birth_month, birth_day;
    scanf("%d-%d-%d", &birth_year, &birth_month, &birth_day);
    printf("请输入今年日期（如：2023-05-20）：");
    int current_year, current_month, current_day;
    scanf("%d-%d-%d", &current_year, &current_month, &current_day);
    int live_days = 0, died_days = 0;
    int month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if((birth_year % 4 == 0 && birth_year % 100 != 0) || birth_year % 400 == 0){
        month[1] = 29;
    }
    live_days += month[birth_month-1] - birth_day;
    for(int i = birth_month; i < 12; i++){
        live_days += month[i];
    }
    died_days = live_days;
    for(int i = birth_year+1; i<=current_year-1;i++){
        if((i % 4 == 0 && i % 100 != 0) || i % 400 == 0){
            live_days += 366;
        }else{
            live_days += 365;
        }
    }
    if((current_year % 4 == 0 && current_year % 100 != 0) || current_year % 400 == 0){
        month[1] = 29;
    }else{
        month[1] = 28;
    }   
    for(int i = 0; i < current_month-1; i++){
        live_days += month[i];
    }
    live_days += current_day;
    printf("你已经活了 %d 天。\n", live_days);
    int died_year = 80+birth_year;
    int died_month = birth_month;
    int died_day = birth_day;
    if((died_year % 4 == 0 && died_year % 100 != 0) || died_year % 400 == 0){
        month[1] = 29;
    }else{
        month[1] = 28;
    }
    for(int i = birth_year+1; i<=died_year-1;i++){
        if((i % 4 == 0 && i % 100 != 0) || i % 400 == 0){
            died_days += 366;
        }else{
            died_days += 365;
        }
    }
    for(int i = 0; i < died_month-1; i++){
        died_days += month[i];
    }
    died_days += died_day;
    printf("你大约还可以活 %d 天。\n", died_days-live_days);
    printf("你已经使用了 %.2f %%的寿命。\n", live_days*100.0/died_days);

}

int main(){
    //test00();
    //test01();
    test02();
    return 0;
}