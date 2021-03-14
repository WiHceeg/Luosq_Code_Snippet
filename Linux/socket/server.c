#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


int main() {

    int lfd = socket(AF_INET, SOCK_STREAM, 0);  //返回用于监听的文件描述符
    if (lfd == -1) {
        perror("socket");
        exit(-1);
    }

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    // inet_pton(AF_INET, "172.27.254.30", saddr.sin_addr.s_addr);
    // inet_pton(AF_INET, "123.56.157.116", saddr.sin_addr.s_addr);
    saddr.sin_addr.s_addr = INADDR_ANY;  //0.0.0.0
    saddr.sin_port = htons(9999);
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1) {
        perror("bind");
        exit(-1);
    }

    ret = listen(lfd, 8);
    if (ret == -1) {
        perror("listen");
        exit(-1);
    }


    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len);    //返回用于和客户端通信的文件描述符
    if (cfd == -1) {
        perror("accept");
        exit(-1);
    }

    //输出客户端的信息
    char clientIP[16];
    inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
    unsigned short clientPort = ntohs(clientaddr.sin_port);
    printf("client IP:   %s\nclient port: %d\n", clientIP, clientPort);

    // 收
    char recvBuf[1024] = {0};
    while (1) {
        ret = read(cfd, recvBuf, sizeof(recvBuf));
        if (ret == -1) {
            perror("read");
            exit(-1);
        } else if (ret > 0) {
            printf("recv client data: %s\n", recvBuf);
        } else if (ret == 0) {
            printf("client closed...\n");
            break;
        }

        // 发
        char data[1024];
        sprintf(data, "hello, I am server, I received your message \"%s\"", recvBuf);
        write(cfd, data, strlen(data));

    }


    close(cfd);
    close(lfd);

    return 0;
}
