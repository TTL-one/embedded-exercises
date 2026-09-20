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
    int cfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(cfd == -1){
        perror("socket");
        return -1;
    }

    struct sockaddr_in SERVER_ADDR;
    SERVER_ADDR.sin_family = AF_INET;
    SERVER_ADDR.sin_port = htons(8888); //host to network,short 目标端口
    /*---- 旧代码（错误） ----
    SERVER_ADDR.sin_addr.s_addr = htonl(INADDR_ANY);
    UDP客户端不需要bind，应该指定目标服务器IP
    ---- 旧代码（错误） ----*/
    SERVER_ADDR.sin_addr.s_addr = inet_addr("127.0.0.1"); //填服务器IP

    while(1){
        char buf[1024];
        printf("input data:");
        fgets(buf, sizeof(buf), stdin);
        /*---- 旧代码（错误） ----
        sendto(cfd, buf, strlen(buf), 0,
               (struct sockaddr *)&SERVER_ADDR, sizeof(SERVER_ADDR));
        sizeof(SERVER_ADDR) ← 第一次调用时已正确，但风格不一致
        ---- 旧代码（错误） ----*/
        if(sendto(cfd, buf, strlen(buf), 0,
                  (struct sockaddr *)&SERVER_ADDR, sizeof(SERVER_ADDR)) == -1){
            perror("sendto");
        }
    }
    close(cfd);
    return 0;
}
