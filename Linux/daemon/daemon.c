#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <string.h>

//守护进程，每2秒获取系统时间，写到磁盘文件中

void work(int num) {
    time_t tm = time(NULL);
    struct tm *loc = localtime(&tm);
    // char buf[1024];
    // sprintf(buf, "%d-%d-%d %d:%d:%d\n",loc->tm_year, loc->tm_mon, loc->tm_mday, loc->tm_hour, loc->tm_min, loc->tm_sec);

    // printf("%s", buf);

    char *str = asctime(loc);
    int fd = open("time.txt", O_RDWR | O_CREAT | O_APPEND, 0664);
    write(fd, str, strlen(str));
    close(fd);
}

int main() {

    pid_t pid = fork();

    if (pid >0) {
        exit(0);
    }

    setsid();

    umask(022);

    chdir("/");

    // int fd = open("/dev/null", O_RDWR);
    // dup2(fd, STDIN_FILENO);
    // dup2(fd, STDOUT_FILENO);
    // dup2(fd, STDERR_FILENO);

    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = work;
    sigemptyset(&act.sa_mask);

    sigaction(SIGALRM, &act, NULL);

    struct itimerval val;
    val.it_value.tv_sec = 2;
    val.it_value.tv_usec = 0;
    val.it_interval.tv_sec = 2;


    setitimer(ITIMER_REAL, &val, NULL);

    while(1) {
        sleep(10);
    }

    return 0;
}
