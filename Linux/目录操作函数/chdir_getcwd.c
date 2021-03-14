#include <unistd.h>
#include <stdio.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main() {

    char buf[128];
    getcwd(buf, sizeof(buf));
    printf("当前工作目录：%s\n", buf);

    int ret = chdir("./aaa");
    if (ret == -1) {
        perror("chdir");
        return -1;
    }

    //创建新文件
    int fd = open("chdir.txt", O_CREAT | O_RDWR, 0664);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    close(fd);

    char newbuf[128];
    getcwd(newbuf, sizeof(newbuf));
    printf("当前工作目录：%s\n", newbuf);


    return 0;
}