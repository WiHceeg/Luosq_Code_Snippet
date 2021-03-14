/*
    实现 ps aux | grep xxx
    子进程：ps aux，子进程结束后，
*/

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>

int main() {

    int pipefd[2];
    int ret = pipe(pipefd);

    if (ret == -1) {
        perror("pipe");
        exit(0);
    }

    pid_t pid = fork();

    if (pid > 0) {
        //父进程

        //关闭写端
        close(pipefd[1]);

        //从管道读
        char buf[1024] = {0};
        
        int len = -1;
        while ((len = read(pipefd[0], buf, sizeof(buf) - 1)) > 0)   {
            //过滤数据输出
            printf("%s", buf);
            memset(buf, 0, 1024);
        }

        wait(NULL);



    } else if (pid == 0) {
        //子进程

        //关闭读端
        close(pipefd[0]);

        //文件描述符重定向
        dup2(pipefd[1], STDOUT_FILENO);

        //ps aux
        execlp("ps", "ps", "aux", NULL);
        perror("execlp");
        exit(0);

    } else {
        perror("fork");
        exit(0);
    }

    return 0;
}
