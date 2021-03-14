/*
    三个窗口卖100张票
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

int tickets = 1000;

//创建一个互斥量
pthread_mutex_t mutex;

void *sellticket(void *arg) {


    while (1) {

        //加锁
        pthread_mutex_lock(&mutex);

        if (tickets > 0) {
            // usleep(50000);
            printf("%ld 正在卖第 %d 张票\n", pthread_self(), tickets);
            tickets--;
        } else {
            pthread_mutex_unlock(&mutex);
            break;
        }

        //解锁
        pthread_mutex_unlock(&mutex);
       


    }
}

int main() {

    //初始化互斥量
    pthread_mutex_init(&mutex, NULL);


    pthread_t tid[3];
    for (int i = 0; i < 3; i++) {
        pthread_create(&tid[i], NULL, sellticket, NULL);
    }

    //回收子线程资源，阻塞
    for (int i = 0; i < 3; i++) {
        pthread_join(tid[i], NULL);
    }

    // //设置线程分离，防止主线程退出后子线程没了
    // for (int i = 0; i < 3; i++) {
    //     pthread_detach(tid[i]);
    // }


    
    //释放互斥量的资源
    pthread_mutex_destroy(&mutex);

    pthread_exit(NULL);
   
    return 0;
}