#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#define LISTENTOCLIENTNUM 5

int main(int argc,char **argv){
    /*
        AF_INET:IPv4
        SOCK_STREAM:TCP
        protocol:0,根据前面两个参数自动选择协议
    */
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd == -1){
        perror("socket");
        return -1;
    }

    int on = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)); //允许端口快速重用

    struct sockaddr_in SERVER_ADDR;
    SERVER_ADDR.sin_family = AF_INET;
    SERVER_ADDR.sin_port = htons(8888); //host to network,short 监听的端口
    SERVER_ADDR.sin_addr.s_addr = htonl(INADDR_ANY); //监听所有的IP

    if(bind(sfd, (struct sockaddr *)&SERVER_ADDR, sizeof(SERVER_ADDR)) == -1){
    /*---- 旧代码（错误） ----
    sizeof(struct sockaddr) ← 类型大小不对，应填实际结构体大小
    ---- 旧代码（错误） ----*/
        perror("bind");
        return -1;
    }

    if(listen(sfd, LISTENTOCLIENTNUM)==-1){
        perror("listen");
        return -1;
    }

    struct sockaddr_in CLIENT_ADDR;
    socklen_t addr_len = sizeof(CLIENT_ADDR); //必须用变量，主要是后面函数要传地址，所以得定义变量
    while(1){
        int cfd = accept(sfd, (struct sockaddr *)&CLIENT_ADDR, &addr_len);
        /*---- 旧代码（错误） ----
        accept失败时直接return -1，服务器直接崩溃退出，
        且sizeof(struct sockaddr)类型大小不对
        ---- 旧代码（错误） ----*/
        if(cfd == -1){
            perror("accept");
            continue; //改为continue，服务器继续等待下一个连接，而不是直接退出
        }
        /*---- 旧代码（错误） ----
        if(!fork()){
            char buf[1024];
            ssize_t recvLen = recv(cfd, buf, sizeof(buf), 0);
            if(recvLen == -1){
                perror("recv");
                return -1;  ← 子进程return导致整个进程退出，而非退出if块
            }
            buf[recvLen] = '\0';
            char ip[16];
            inet_ntop(AF_INET, &CLIENT_ADDR.sin_addr.s_addr, ip, sizeof(ip));
            printf("recv client %s data:%s\n",ip,buf);
            // ← 这里recv完直接落到外层while(1)，会再次accept，但listen socket在父进程里
            // ← 且子进程recv完后没有close(cfd)，造成fd泄漏
        }else{
            close(cfd);  ← 父进程关闭cfd是对的
        }
        ---- 旧代码（错误） ----*/
        if(!fork()){
            // 子进程：负责与客户端通信
            char buf[1024];
            ssize_t recvLen = recv(cfd, buf, sizeof(buf), 0);
            if(recvLen == -1){
                perror("recv");
                close(cfd);
                exit(0); //子进程出错退出，不用return -1（会退出整个进程）
            }
            if(recvLen == 0){
                // 对端关闭连接
                close(cfd);
                exit(0);
            }
            buf[recvLen] = '\0';
            char ip[16];
            inet_ntop(AF_INET, &CLIENT_ADDR.sin_addr, ip, sizeof(ip)); //sin_addr直接传，不用取地址
            printf("recv client %s data:%s\n",ip,buf);
            close(cfd); //子进程处理完通信，关闭cfd
            exit(0); //子进程完成任务，正常退出
        }else{
            // 父进程：不参与通信，只关闭cfd，继续等待下一个连接
            close(cfd);
        }
    }
    close(sfd);
    return 0;
}
