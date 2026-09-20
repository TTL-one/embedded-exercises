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
        SOCK_STREAM:TCP
        protocol:0,根据前面两个参数自动选择协议
    */
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    if(cfd == -1){
        perror("socket");
        return -1;
    }

    struct sockaddr_in SERVER_ADDR;
    SERVER_ADDR.sin_family = AF_INET;
    SERVER_ADDR.sin_port = htons(8888); //host to network,short 访问的端口
    /*---- 旧代码（错误） ----
    SERVER_ADDR.sin_addr.s_addr = htonl(INADDR_ANY); //监听所有的IP
    客户端不需要绑定任何本地地址，
    应该填服务器IP，这里填127.0.0.1即本机测试
    ---- 旧代码（错误） ----*/
    SERVER_ADDR.sin_addr.s_addr = inet_addr("127.0.0.1"); //填服务器IP

    if(connect(cfd, (struct sockaddr *)&SERVER_ADDR, sizeof(SERVER_ADDR)) == -1){
    /*---- 旧代码（错误） ----
    sizeof(struct sockaddr) ← 类型大小不对，应填实际结构体大小
    ---- 旧代码（错误） ----*/
        perror("connect");
        return -1;
    }
    while(1){
        char buf[1024];
        printf("input data:");
        fgets(buf, sizeof(buf), stdin);
        send(cfd, buf, strlen(buf), 0);
    }
    close(cfd);
    return 0;
}
