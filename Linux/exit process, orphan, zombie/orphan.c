#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

    pid_t pid = fork();

    // 判断是父进程还是子进程

    if (pid > 0) {
        //父进程
        // printf("pid: %d\n", pid);   //打印子进程id
        printf("parent, pid: %d, ppid: %d\n", getpid(), getppid());
    } else if (pid == 0) {
        //子进程
        sleep(1);
        printf("child, pid: %d, ppid: %d\n", getpid(), getppid());
    }

    //父子共享
    for (int i = 0; i < 3; i++) {
        printf("i: %d, pid: %d\n", i, getpid());
        // sleep(1);
    }



    return 0;
}
