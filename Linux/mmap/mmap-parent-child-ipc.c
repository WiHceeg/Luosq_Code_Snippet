#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>

int main() {

    int fd = open("test.txt", O_RDWR);
    int size = lseek(fd, 0, SEEK_END);

    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }

    //创建子进程
    pid_t pid = fork();
    if (pid > 0) {
        //父进程
        wait(NULL);
        char buf[64];
        strcpy(buf, (char *) ptr);
        printf("read data: %s\n", buf);

    } else {
        //子进程
        strcpy((char *) ptr, "hello, son!");

    }

    //关闭内存映射区
    munmap(ptr, size);

    return 0;
}
