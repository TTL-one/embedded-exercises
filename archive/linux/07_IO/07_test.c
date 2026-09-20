#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]){

    int fd=0,fd2=0;

    fd = open(argv[1],O_RDONLY);
    fd2 = open(argv[1],O_RDONLY);

    char buffer,buffer2;
    read(fd,&buffer,1);
    read(fd2,&buffer2,1);

    printf("fd1 read: %c\n",buffer);
    printf("fd2 read: %c\n",buffer2);

    int fd3 = dup(fd);
    char buffer3;
    read(fd3,&buffer3,1);
    printf("fd3 read: %c\n",buffer3);

    close(fd);
    close(fd2);
    close(fd3);

    return 0;
}