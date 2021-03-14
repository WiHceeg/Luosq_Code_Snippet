#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {

    int fd = socket(AF_INET, SOCK_STREAM, 0);  //返回用于监听的文件描述符
    if (fd == -1) {
        perror("socket");
        exit(-1);
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, "123.56.157.116", &serveraddr.sin_addr.s_addr);
    // inet_pton(AF_INET, "172.27.254.30", &serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(9999);
    int ret = connect(fd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (ret == -1) {
        perror("connect");
        exit(-1);
    }

    while (1) {

        char sendBuf[1000] = {0};
        printf("send：");
        fgets(sendBuf, sizeof(sendBuf), stdin);

        write(fd, sendBuf, strlen(sendBuf) + 1);

        int len = read(fd, sendBuf, sizeof(sendBuf));
        if (len == -1) {
            perror("read");
            exit(-1);
        } else if (len > 0) {
            printf("recv server data: %s\n", sendBuf);
        } else if (len == 0) {
            printf("server closed...\n");
        }
    }
    close(fd);



    return 0;
}
