/*
    8个线程操作同一个全局变量
    3个线程不定时写这个全局变量，5个线程不定时读这个全局变量
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

int num = 1;
pthread_rwlock_t rwlock;

void* writeNum(void *arg) {
    while (1) {
        pthread_rwlock_wrlock(&rwlock);

        num++;
        printf("++write, tid: %ld, num: %d\n", pthread_self(), num);

        pthread_rwlock_unlock(&rwlock);

        usleep(100);
    }
    return NULL;
}

void* readNum(void *arg) {
    while (1) {
        pthread_rwlock_rdlock(&rwlock);
        
        printf("===read, tid: %ld, num: %d\n", pthread_self(), num);

        pthread_rwlock_unlock(&rwlock);

        usleep(100);
    }
    return NULL;
}

int main() {

    pthread_rwlock_init(&rwlock, NULL);

    pthread_t wtid[3], rtid[5];

    for (int i = 0; i < 3; i++) {
        pthread_create(&wtid[i], NULL, writeNum, NULL);
    }
    for (int i = 0; i < 5; i++) {
        pthread_create(&rtid[i], NULL, readNum, NULL);
    }    

    for (int i = 0; i < 3; i++) {
        pthread_detach(wtid[i]);
    }
    for (int i = 0; i < 5; i++) {
        pthread_detach(rtid[i]);
    }    

    pthread_exit(NULL);

    pthread_rwlock_destroy(&rwlock);

    return 0;

}
