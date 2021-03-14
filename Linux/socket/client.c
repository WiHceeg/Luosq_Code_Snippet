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

    char recvBuf[1024] = {0};
    while (1) {

        char data[1000];
        printf("send：");
        fgets(data, sizeof(data), stdin);   //fgets能存空格，而且不会溢出
//         scanf("%s", data);
        write(fd, data, strlen(data));
        sleep(1);

        ret = read(fd, recvBuf, sizeof(recvBuf));
        if (ret == -1) {
            perror("read");
            exit(-1);
        } else if (ret > 0) {
            printf("recv server data: %s\n", recvBuf);
        } else if (ret == 0) {
            printf("server closed...\n");
        }
    }
    close(fd);



    return 0;
}
