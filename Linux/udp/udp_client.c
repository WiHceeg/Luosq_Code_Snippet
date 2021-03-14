 
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {

    int fd = socket(AF_INET, SOCK_DGRAM, 0);  //返回用于监听的文件描述符
    if (fd == -1) {
        perror("socket");
        exit(-1);
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, "123.56.157.116", &serveraddr.sin_addr.s_addr);
//     inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(9999);

    int num = 0;
    while (1) {

        char sendBuf[128] = {0};
        sprintf(sendBuf, "hello, I am client, num %d", num++);

        sendto(fd, sendBuf, strlen(sendBuf) + 1, 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

        int num = recvfrom(fd, sendBuf, sizeof(sendBuf), 0, NULL, NULL);
        printf("server say: %s\n", sendBuf);

        sleep(1);
    }
    close(fd);



    return 0;
}
