#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main() {

    //1. 判断有名管道文件是否存在
    int ret = access("fifo1", F_OK);
    if (ret == -1) {
        printf("fifo1不存在，创建fifo1\n");
        ret = mkfifo("fifo1", 0664);
        if (ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    ret = access("fifo2", F_OK);
    if (ret == -1) {
        printf("fifo2不存在，创建fifo2\n");
        ret = mkfifo("fifo2", 0664);
        if (ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    //以只读的方式打开管道fifo1
    int fdr = open("fifo1", O_RDONLY);
    if (fdr == -1) {
        perror("open");
        exit(0);
    }
    printf("打开fifo1成功，等待读取...\n");

    //以只写的方式打开管道fifo2
    int fdw = open("fifo2", O_WRONLY);
    if (fdw == -1) {
        perror("open");
        exit(0);
    }
    printf("打开fifo2成功，等待写入...\n");


    char buf[128];

    //循环读写数据
    while(1) {


        memset(buf, 0, 128);
        ret = read(fdr, buf, 128);
        if (ret <= 0) {
            perror("read");
            break;
        }
        printf("buf: %s\n", buf);


        memset(buf, 0, 128);

        fgets(buf, 128, stdin);

        int ret = write(fdw, buf, strlen(buf));
        if (ret == -1) {
            perror("write");
            exit(0);
        }
    }

    close(fdr);
    close(fdw);

    return 0;
}
