#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>


/*
int setitimer(int which, const struct itimerval *new_value,
                struct itimerval *old_value);
*/


//3秒以后，每2秒定时一次
//这个程序3秒后就退出了，看不到后面的2秒了，因为没用信号捕获
int main() {

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
