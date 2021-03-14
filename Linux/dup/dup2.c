#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


int main() {

    int fd1 = open("1.txt", O_RDWR | O_CREAT, 0664);
    if (fd1 == -1) {
        perror("open");
        return -1;
    }

    int fd2 = open("2.txt", O_RDWR | O_CREAT, 0664);
    if (fd2 == -1) {
        perror("open");
        return -1;
    }

    printf("fd1: %d, fd2: %d\n", fd1, fd2);

    int fd3 = dup2(fd1, fd2);
    if (fd3 == -1) {
        perror("open");
        return -1;
    }

    //通过fd2去写数据，实际操作1.txt

    char *str = "hello,dup2";
    int len = write(fd2, str, strlen(str));
    if (len == -1) {
        perror("write");
        return -1;
    }

    printf("fd1: %d, fd2: %d, fd3: %d\n", fd1, fd2, fd3);

    close(fd1);
    close(fd2);



    return 0;
}
