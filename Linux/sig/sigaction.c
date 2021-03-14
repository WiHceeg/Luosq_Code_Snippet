#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

/*
    #include <signal.h>

    int sigaction(int sig, const struct sigaction *restrict act,
        struct sigaction *restrict oact);


    struct sigaction {
        void     (*sa_handler)(int);
        void     (*sa_sigaction)(int, siginfo_t *, void *);
        sigset_t   sa_mask;
        int        sa_flags;
        void     (*sa_restorer)(void);
    };
*/

void myalarm(int num) {
    printf("捕捉到的信号编号是: %d\n", num);
    printf("xxxxx\n");
}

//3秒以后，每2秒定时一次
//信号捕捉
int main() {

    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = myalarm;
    sigemptyset(&act.sa_mask);
    

    //注册信号捕捉
    sigaction(SIGALRM, &act, NULL);

    struct itimerval new_value;

    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;

    new_value.it_value.tv_sec = 3;
    new_value.it_value.tv_usec = 0;

    int ret = setitimer(ITIMER_REAL, &new_value, NULL); //非阻塞的
    
    printf("定时器开始了...\n");


    if (ret == -1) {
        perror("setitimer");
        exit(0);
    }

    while(1) {
        ;
    }


    return 0;
}
