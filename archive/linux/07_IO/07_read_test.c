#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

/*
SYNOPSIS
       #include <unistd.h>

       ssize_t read(int fd, void *buf, size_t count);

DESCRIPTION
       read()  attempts to read up to count bytes from file descriptor fd into
       the buffer starting at buf.
*/

int main(int argc, char *argv[]){
    int fd;
    char buffer[100];
    if(argc<2){
        printf("Usage:%s <file>\n",argv[0]); //<file>尖括号意味着这个参数不可省略
        return -1;
    }
    fd = open(argv[1],O_RDONLY, 0777);//oepn_readandwrite
    if(fd<0){
        printf("Error opening file\n");
        printf("Error: %s\n", strerror(errno));
        perror("open");
        return -1;
    }
    printf("File opened successfully with file descriptor: %d\n", fd);

    int len = read(fd,buffer,sizeof(buffer)-1);
    if(len == -1){
        perror("read");
        close(fd);
        return -1;
    }
    buffer[len] = '\0';
    printf("Content read from file:\n%s\n", buffer);

    close(fd);
    return 0;

}