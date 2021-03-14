#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main() {

    int fd = open("test", O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(0);
    }

    while (1) {
        char buf[1024] = {0};
        int len = read(fd, buf, sizeof(buf));
        if (len == 0) {
             printf("写端断开连接了...\n");
             break;
        }
        printf("recv buf: %s\n", buf);
    }

    return 0;
}
