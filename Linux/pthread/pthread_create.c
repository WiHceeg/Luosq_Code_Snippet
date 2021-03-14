#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void *callback(void *arg) {
    printf("child thread...\n");
    printf("arg value: %d\n", *(int*)arg);
    return NULL;
}

int main() {

    pthread_t tid;

    int num = 10;

    int ret = pthread_create(&tid, NULL, callback, (void*)&num);
    if (ret != 0) {
        char *errstr = strerror(ret);
        printf("error: %s\n", errstr);
    }


    for (int i = 0; i < 5; i++) {
        printf("%d ", i);
    }

    sleep(1);

    return 0;
}