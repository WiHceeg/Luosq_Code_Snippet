#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void* callback(void *arg) {
    printf("child thread id: %ld\n", pthread_self());
    for (int i = 0; i < 5; i++) {
        printf("child: %d\n", i);
    }
    return NULL;
}


int main() {

    pthread_t tid;

    int ret = pthread_create(&tid, NULL, callback, NULL);
    if (ret != 0) {
        char *errstr = strerror(ret);
        printf("error: %s\n", errstr);
    }

    printf("tid: %ld, main thread id: %ld\n", tid, pthread_self());

    //取消线程
    pthread_cancel(tid);

    for (int i = 0; i < 5; i++) {
        printf("main: %d\n", i);
    }

    pthread_exit(NULL);

    return 0;
}