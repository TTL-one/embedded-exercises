#include <stdio.h>

// argc argument count 
// argv argument value or vector
// ./hello 123
// argc = 2
// argv[0] = "./hello"
// argv[1] = "123"

int main(int argc, char *argv[]) {

    char name[100];
    int age = 18;
    
    //scanf("%[^,],%d", name, &age);
    //scanf("%[^ ] %d", name, &age);
    scanf("%s,%d",name,&age);


    printf("%s %d\n",name, age);
    /*
    if(argc >=2){
        printf("Hello %s!\n",argv[1]);
    }else{
        printf("Hello World！\n");
    }
    */
    return 0;
}