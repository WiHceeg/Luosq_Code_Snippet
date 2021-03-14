#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
/*
对自定义的信号集的操作
int sigemptyset(sigset_t *set);

int sigfillset(sigset_t *set);

int sigaddset(sigset_t *set, int signum);

int sigdelset(sigset_t *set, int signum);

int sigismember(const sigset_t *set, int signum);
*/

int main() {

    sigset_t set;
    
    sigemptyset(&set);
    
    int ret = sigismember(&set, SIGINT);
    if (ret == 0) {
        printf("SIGINT 不阻塞\n");
    } else if (ret == 0) {
        printf("SIGINT 阻塞\n");
    } else {
        perror("sigismember");
        exit(0);
    }

    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);

    ret = sigismember(&set, SIGINT);
    if (ret == 0) {
        printf("SIGINT 不阻塞\n");
    } else if (ret == 1) {
        printf("SIGINT 阻塞\n");
    } else {
        perror("sigismember");
        exit(0);
    }

    ret = sigismember(&set, SIGQUIT);
    if (ret == 0) {
        printf("SIGQUIT 不阻塞\n");
    } else if (ret == 1) {
        printf("SIGQUIT 阻塞\n");
    } else {
        perror("sigismember");
        exit(0);
    }


    sigdelset(&set, SIGQUIT);

    ret = sigismember(&set, SIGQUIT);
    if (ret == 0) {
        printf("SIGQUIT 不阻塞\n");
    } else if (ret == 1) {
        printf("SIGQUIT 阻塞\n");
    } else {
        perror("sigismember");
        exit(0);
    }

    return 0;
}
