#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>

//使用内存映射实现文件拷贝的功能

int main() {

    int fd = open("english.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(0);
    }

    //获取原始文件的大小
    int len = lseek(fd, 0, SEEK_END);

    //创建一个新文件并扩展
    int fd1 = open("cpy.txt", O_RDWR | O_CREAT, 0664);
    if(fd1 == -1) {
        perror("open");
        exit(0);
    }
    truncate("cpy.txt", len);
    write(fd1, " ", 1);


    void *ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }

    void *ptr1 = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);

    if (ptr1 == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }

    //内存拷贝
    memcpy(ptr1, ptr, len);

    //释放资源
    munmap(ptr1, len);
    munmap(ptr, len);

    close(fd1);
    close(fd);

    return 0;
}
