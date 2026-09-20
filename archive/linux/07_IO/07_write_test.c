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
    if(argc<3){
        printf("Usage:%s <file> <string1> <string2> ...\n",argv[0]); //<file>尖括号意味着这个参数不可省略
        return -1;
    }
    fd = open(argv[1],O_RDWR|O_CREAT, 0777);//oepn_readandwrite
    if(fd==-1){
        printf("Error opening file\n");
        printf("Error: %s\n", strerror(errno));
        perror("open");
        return -1;
    }
    printf("File opened successfully with file descriptor: %d\n", fd);


    for(int i=2;i<argc;i++){
        int len = write(fd, argv[i], strlen(argv[i]));
        if(len != strlen(argv[i])){
            printf("Error writing to file\n");
            printf("Error: %s\n", strerror(errno));
            perror("write");
            close(fd);
            return -1;
        }
        write(fd, "\r\n", 2);
    }
    
    lseek(fd, 3, SEEK_SET);//将文件指针移动到文件开头
    write(fd, "abc", 3);//覆盖原来的内容

    close(fd);
    return 0;

}