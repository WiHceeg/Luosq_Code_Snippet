#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

int main() {

    // 子进程发数据给父进程

    // fork之前创建管道
    int pipefd[2];
    int ret = pipe(pipefd);
    if (ret == -1) {
        perror("pipe");
        exit(0);
    }

    // 创建子进程
    pid_t pid = fork();
    if (pid > 0) {
        //父进程
        printf("I am parent process, pid: %d\n", getpid());

        char buf[1024] = {0};

        while(1) {
            int len = read(pipefd[0], buf, sizeof(buf));
            printf("parent recv: %s, pid: %d\n", buf, getpid());

            char *str = "hello, I am parent";
            write(pipefd[1], str, strlen(str));   
            sleep(1);
        }

    
    } else if (pid == 0) {
        // 子进程
        printf("I am child process, pid: %d\n", getpid());

        char buf[1024] = {0};


        while (1) {
            // 向管道写入数据
            char *str = "hello, i am child";
            write(pipefd[1], str, strlen(str));   
            sleep(1);

            int len = read(pipefd[0], buf, sizeof(buf));
            printf("child recv: %s, pid: %d\n", buf, getpid());

        }
 
    }


    return 0;
}
