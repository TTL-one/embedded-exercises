#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

/*
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);
The  argument  flags  must  include  one of the following access modes:
O_RDONLY, O_WRONLY, or O_RDWR.  These request opening  the  file  read-
only, write-only, or read/write, respectively.
*/

int main(int argc, char *argv[]){
    int fd;
    if(argc<2){
        printf("Usage:%s <file>\n",argv[0]); //<file>尖括号意味着这个参数不可省略
        return -1;
    }
    fd = open(argv[1],O_RDWR|O_CREAT|O_TRUNC, 0777);//oepn_readandwrite
    if(fd==-1){
        printf("Error opening file\n");
        printf("Error: %s\n", strerror(errno));
        perror("open");
        return -1;
    }
    printf("File opened successfully with file descriptor: %d\n", fd);
    while(1){
        sleep(10);
    }
    close(fd);
    return 0;

}

// ./07_open_test vim.txt &
//[1] 43318
//File opened successfully with file descriptor: 3