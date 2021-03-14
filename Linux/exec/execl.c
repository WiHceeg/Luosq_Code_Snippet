/*
       #include <unistd.h>

       extern char **environ;

       int execl(const char *pathname, const char *arg, ...);
*/

#include <unistd.h>
#include <stdio.h>


int main() {

    //创建一个子进程，在子进程中执行exec函数族的函数

    pid_t pid = fork();

    if (pid > 0) {
        printf("parent, pid: %d\n", getpid());
        sleep(1);
    } else if (pid == 0) {
        // execl("hello", "hello", NULL);

        execl("/bin/ps", "ps", "aux", NULL);    //执行shell命令
        
        printf("child, pid: %d\n", getpid());
    }

    for (int i = 0; i < 3; i++) {
        printf("i = %d, pid = %d\n", i, getpid());
    }

    return 0;
}