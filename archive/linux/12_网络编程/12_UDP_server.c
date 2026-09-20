#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define LISTENTOCLIENTNUM 5

int main(int argc,char **argv){
    /*
        AF_INET:IPv4
        SOCK_DGRAM:UDP
        protocol:0,根据前面两个参数自动选择协议
    */
    int sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sfd == -1){
        perror("socket");
        return -1;
    }

    struct sockaddr_in SERVER_ADDR;
    SERVER_ADDR.sin_family = AF_INET;
    SERVER_ADDR.sin_port = htons(8888); //host to network,short 监听端口
    SERVER_ADDR.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sfd, (struct sockaddr *)&SERVER_ADDR, sizeof(SERVER_ADDR)) == -1){
        perror("bind");
        return -1;
    }

    /*---- 旧代码（错误） ----
    struct sockaddr_in CLIENT_ADDR;
    while(1){
        if(!fork()){
            // UDP是无连接的，每次recvfrom都是独立的请求，
            // 不需要fork来"服务客户端"，fork只会造成混乱
            char buf[1024];
            ssize_t recvLen = recvfrom(sfd, buf, sizeof(buf), 0,
                                       (struct sockaddr *)&CLIENT_ADDR, sizeof(CLIENT_ADDR));
            if(recvLen == -1){
                perror("recv");
                return -1;
            }
            buf[recvLen] = '\0';
            char ip[16];
            inet_ntop(AF_INET, &CLIENT_ADDR.sin_addr.s_addr, ip, sizeof(ip));
            printf("recv client %s data:%s\n",ip,buf);
        }else{
            // 父进程每次循环都在else分支执行close(sfd)，
            // 第一次循环就关闭了sfd，后续recvfrom就操作已关闭的fd了
            close(sfd);
        }
    }
    ---- 旧代码（错误） ----*/
    struct sockaddr_in CLIENT_ADDR;
    socklen_t addr_len = sizeof(CLIENT_ADDR);
    while(1){
        // UDP无需fork，直接循环接收即可，每个recvfrom都是独立的请求
        char buf[1024];
        ssize_t recvLen = recvfrom(sfd, buf, sizeof(buf), 0,
                                   (struct sockaddr *)&CLIENT_ADDR, &addr_len);
        /*---- 旧代码（错误） ----
        sizeof(CLIENT_ADDR) ← 应传指针，且recvLen==0时应正常关闭退出
        ---- 旧代码（错误） ----*/
        if(recvLen == -1){
            perror("recvfrom");
            continue;
        }
        if(recvLen == 0){
            // 对端关闭了连接（UDP对端退出）
            close(sfd);
            break;
        }
        buf[recvLen] = '\0';
        char ip[16];
        inet_ntop(AF_INET, &CLIENT_ADDR.sin_addr, ip, sizeof(ip)); //sin_addr直接传
        printf("recv client %s data:%s\n", ip, buf);

        // 可选：回复客户端
        char reply[] = "server got it\n";
        sendto(sfd, reply, strlen(reply), 0,
               (struct sockaddr *)&CLIENT_ADDR, sizeof(CLIENT_ADDR));
    }
    close(sfd);
    return 0;
}
