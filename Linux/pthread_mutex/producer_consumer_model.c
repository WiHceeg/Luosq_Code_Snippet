
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

pthread_mutex_t mutex;

struct Node{
    int num;
    struct Node *next;
};

struct Node *head = NULL;


void* producer(void *arg) {

    while(1) {
        pthread_mutex_lock(&mutex);

        struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->next = head;
        head = newNode;
        newNode->num = rand() %1000;
        printf("add node, num: %d, tid: %ld\n", newNode->num, pthread_self());

        pthread_mutex_unlock(&mutex);
        usleep(100);
    }

    return NULL;
}

void* consumer(void *arg) {

    while(1) {
        pthread_mutex_lock(&mutex);

        if (head != NULL) {
            struct Node* tmp = head;
            head = head->next;
            printf("del node, num: %d, tid: %ld\n",tmp->num, pthread_self());
            free(tmp);
        }

        pthread_mutex_unlock(&mutex);
      
        usleep(100);
    }

    return NULL;
}



int main() {

    pthread_mutex_init(&mutex, NULL);

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

    pthread_exit(NULL);


    return 0;
}
