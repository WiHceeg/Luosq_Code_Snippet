/*
    #include <signal.h>

    typedef void (*sighandler_t)(int);  //函数指针

    sighandler_t signal(int signum, sighandler_t handler);

*/

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


void myalarm(int num) {
    printf("捕捉到的信号编号是: %d\n", num);
    printf("xxxxx\n");
}

/*
int setitimer(int which, const struct itimerval *new_value,
                struct itimerval *old_value);
*/


//3秒以后，每2秒定时一次
//信号捕捉
int main() {

    //注册信号捕捉
    // signal(SIGALRM, SIG_IGN);
    // signal(SIGALRM, SIG_DFL);
    signal(SIGALRM, myalarm);

    struct itimerval new_value;

    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;

    new_value.it_value.tv_sec = 3;
    new_value.it_value.tv_usec = 0;

    int ret = setitimer(ITIMER_REAL, &new_value, NULL);
    
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
