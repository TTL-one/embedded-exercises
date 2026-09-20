#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]){

    int fd=0;
    fd = open(argv[1],O_RDWR|O_CREAT|O_TRUNC,0777);
    dup2(fd,1);
    printf("Hello World.\n");
    close(fd);
    return 0;
}