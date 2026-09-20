#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdlib.h>

int main(int argc,char **argv){

    int fd = open("/dev/input/event0",O_RDONLY | O_NONBLOCK);
    if(fd < 0){
        perror("open");
        return -1;
    }

/*
    select() allows a program to monitor multiple file descriptors, waiting
    until one or more of the file descriptors become "ready" for some class
    of I/O operation (e.g., input possible).  A file descriptor is  consid‐
    ered  ready  if it is possible to perform a corresponding I/O operation
    (e.g., read(2), or a sufficiently small write(2)) without blocking.


    **参数**
    以下是 `select()` 的参数说明：

    **readfds**  
    这个集合中的文件描述符会被监视，看它们是否**可读**。当对一个文件描述符执行读操作不会阻塞时，它就是可读的；特别地，当到达文件末尾时，文件描述符也视为可读。  
    `select()` 返回后，`readfds` 中只会保留那些已可读的文件描述符，其余都会被清除。

    **writefds**  
    这个集合中的文件描述符会被监视，看它们是否**可写**。当对一个文件描述符执行写操作不会阻塞时，它就是可写的。但要注意，即使文件描述符显示为可写，一次大的写操作仍然可能阻塞。  
    `select()` 返回后，`writefds` 中只会保留那些已可写的文件描述符，其余都会被清除。

    **exceptfds**  
    这个集合中的文件描述符会被监视，看是否发生**异常条件**。一些异常条件的例子可以参考 `poll(2)` 中对 `POLLPRI` 的讨论。  
    `select()` 返回后，`exceptfds` 中只会保留那些发生了异常条件的文件描述符，其余都会被清除。

    **nfds**  
    这个参数应设置为三个集合中**最大文件描述符编号加 1**。内核会检查每个集合中编号小于该上限的文件描述符（但请注意 BUGS 一节中提到的限制）。

    **timeout**  
    `timeout` 参数是一个 `timeval` 结构体（如下所示），用于指定 `select()` 阻塞等待文件描述符就绪的最长时间。调用将一直阻塞，直到以下任一情况发生：

    - 某个文件描述符变为就绪状态；
    - 被信号处理函数中断；
    - 指定的超时时间耗尽。

    **返回值**

    成功时，`select()` 和 `pselect()` 返回三个已返回的描述符集合中包含的文件描述符总数（即 `readfds`、`writefds`、`exceptfds` 中被置位的比特数总和）。如果在任何文件描述符就绪之前超时时间耗尽，返回值可能为 0。
    出错时，返回 -1，并设置 `errno` 来指示错误；此时文件描述符集合保持不变，`timeout` 的值变得未定义。

    int select(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, struct timeval *timeout);


    **FD_ZERO()**  
    该宏用于清空集合（将集合中所有文件描述符移除）。它应作为初始化文件描述符集合的第一步使用。

    **FD_SET()**  
    该宏将文件描述符 `fd` 添加到集合中。如果添加的文件描述符已经存在于集合中，该操作无任何效果，也不会产生错误。

    **FD_CLR()**  
    该宏从集合中移除文件描述符 `fd`。如果要移除的文件描述符原本就不在集合中，该操作无任何效果，也不会产生错误。

    **FD_ISSET()**  
    `select()` 会根据下文描述的规则修改集合的内容。调用 `select()` 之后，可以使用 `FD_ISSET()` 宏来测试某个文件描述符是否仍然存在于集合中。如果文件描述符 `fd` 存在于集合中，`FD_ISSET()` 返回非零值，否则返回零。
*/ 

    fd_set readfds;
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);


    int ret = select(fd+1,&readfds,NULL,NULL,&timeout);

    if(ret < 0){
        perror("select");
        return -1;
    }
    else if(ret == 0){
        printf("select timeout\n");
    }
    else{
        if(FD_ISSET(fd,&readfds)){
            struct input_event event;
            while(read(fd,&event,sizeof(event)) == sizeof(event)){
                printf("get event: type = 0x%x, code = 0x%x, value = 0x%x\n", event.type, event.code, event.value);
            }
        }
    }




    return 0;
}