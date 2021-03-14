#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main() {

    // 判断管道文件是否存在
    int ret = access("test", F_OK);
    
    if (ret == -1) {
        printf("管道不存在，创建管道\n");


        int ret = mkfifo("test", 0664);

        if (ret == -1) {
            perror("mkfifo");
            exit(0);
        }

    }


    int fd = open("test", O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(0);
    }


    for (int i = 0; i < 100; i++) {
        char buf[1024];
        sprintf(buf, "hello, %d\n", i);
        printf("write data: %s\n", buf);
        write(fd, buf, strlen(buf));
        sleep(1);
    }

    close(fd);

    return 0;
}
