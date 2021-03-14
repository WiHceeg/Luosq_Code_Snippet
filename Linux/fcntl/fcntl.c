#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main() {

    //1. 复制文件描述符
    // int fd = open("1.txt", O_RDONLY);
    // int ret = fcntl(fd, F_DUPFD);

    //2. 修改或则获取文件状态flag
    int fd = open("1.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    //先获取flag
    int flag = fcntl(fd, F_GETFL);
    if (flag == -1) {
        perror("fcntl");
        return -1;
    }

    int ret = fcntl(fd, F_SETFL, flag | O_APPEND);
    if (ret == -1) {
        perror("fcntl");
        return -1;
    }

    char *str = "nihao";
    write(fd, str, strlen(str));

    close(fd);

    return 0;
}
