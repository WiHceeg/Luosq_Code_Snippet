#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
    #include <signal.h>

    int sigprocmask(int how, const sigset_t *restrict set,
        sigset_t *restrict oset);
    int sigpending(sigset_t *set);
 */


//把所有的常规信号1-31的未决状态打印到屏幕
//思路，设置某些信号是阻塞的，通过键盘产生它们
int main() {

    //2, 3号信号设置为阻塞，可以通过ctrl+c, ctrl+
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);

    //设置内核中的阻塞信号集
    sigprocmask(SIG_BLOCK, &set, NULL);


    int num = 0;
    while(1) {
        //获取当前未决信号集的数据
        sigset_t pendingset;
        sigemptyset(&pendingset);
        sigpending(&pendingset);

        //遍历前32位
        for (int i = 1; i <= 32; i++) {
            int ret = sigismember(&pendingset, i);
            if(ret == 1) {
                printf("1");
            } else if (ret == 0) {
                printf("0");
            } else {
                perror("sigismember");
                exit(0);
            }
        }
        printf("\n");
        sleep(1);

        num++;
        if (num == 10) {
            //解除阻塞
            sigprocmask(SIG_UNBLOCK, &set, NULL);
        }
    }

    return 0;
}
