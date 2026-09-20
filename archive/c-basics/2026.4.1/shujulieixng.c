#include <stdio.h>
#include<math.h>
int main(){
    printf("%f\n", pow(1,2)+pow(2,3)+pow(3,4)+pow(4,5)+pow(5,6));
    unsigned long long int sum=0;
    unsigned long long int temp=0;
    unsigned long long int weight = 0;
    int i;
    for(i=0;i<64;i++){
        temp = pow(2,i);
        sum =sum+temp;
    }
    weight = sum /25000;
    printf("应给与%llu\n",sum);
    printf("应给与%llu斤\n",weight );
    return 0;
}