#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

int value = 10;


void *callback(void *arg) {
    printf("child thread id: %ld\n", pthread_self());
    // sleep(3);
    // return NULL;
    pthread_exit((void*)&value);        //return (void*)&value;一样
}


int main() {

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);

    if (ret != 0) {
        char *errstr = strerror(ret);
        printf("error: %s\n", errstr);
    }


    for (int i = 0; i < 5; i++) {
        printf("%d\n", i);
    }

    printf("tid: %ld, main thread id: %ld\n", tid, pthread_self());

    //主线程调用pthread_join()回收子线程的资源
    int *thread_retval;
    ret = pthread_join(tid, (void **)&thread_retval);

    if (ret != 0) {
        char *errstr = strerror(ret);
        printf("error: %s\n", errstr);
    }

    printf("exit data: %d\n", *thread_retval);

    printf("回收子线程资源成功\n");

    pthread_exit(NULL);

    return 0;
}