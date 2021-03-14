/*
       #include <pthread.h>

       pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

       int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *cond_attr);
       int pthread_cond_signal(pthread_cond_t *cond);

       int pthread_cond_broadcast(pthread_cond_t *cond);

       int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

       int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);
       int pthread_cond_destroy(pthread_cond_t *cond);
*/


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

pthread_mutex_t mutex;

pthread_cond_t cond;

struct Node{
    int num;
    struct Node *next;
};

struct Node *head = NULL;


void* producer(void *arg) {
    //不断创建新节点加到链表中
    while(1) {
        pthread_mutex_lock(&mutex);

        struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->next = head;
        head = newNode;
        newNode->num = rand() %1000;
        printf("add node, num: %d, tid: %ld\n", newNode->num, pthread_self());

        //只要生产了一个就通知消费者消费
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
        usleep(100);
    }

    return NULL;
}

void* consumer(void *arg) {
    //按照自己的想法这样写有问题，还没弄明白
    // while(1) {
    //     pthread_mutex_lock(&mutex);

    //     if (head == NULL) {
    //         pthread_cond_wait(&cond, &mutex);   //这里会自动解mutex，不阻塞时，继续向下执行，重新加锁
    //     }

    //     struct Node* tmp = head;
    //     head = head->next;
    //     printf("del node, num: %d, tid: %ld\n",tmp->num, pthread_self());
    //     free(tmp);
        

    //     pthread_mutex_unlock(&mutex);
      
    //     usleep(100);
    // }
    while(1) {
        pthread_mutex_lock(&mutex);
        struct Node* tmp = head;

        if (head != NULL) {
            head = head->next;
            printf("del node, num: %d, tid: %ld\n",tmp->num, pthread_self());
            free(tmp);
            pthread_mutex_unlock(&mutex);
            usleep(100);
        } else {
            pthread_cond_wait(&cond, &mutex);   //这里会自动解mutex，不阻塞时，继续向下执行，重新加锁
            pthread_mutex_unlock(&mutex);

        }

    }

    return NULL;
}



int main() {

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    //5个生产者，5个消费者
    pthread_t ptid[5], ctid[5];

    for (int i = 0; i < 5; i++) {
        pthread_create(&ptid[i], NULL, producer, NULL);
        pthread_create(&ctid[i], NULL, consumer, NULL);
    }


    for (int i = 0; i < 5; i++) {
        pthread_join(ptid[i], NULL);
        pthread_join(ctid[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_exit(NULL);

    return 0;
}
