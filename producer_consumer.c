#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

// gcc -g -pthread producer_consumer_2.c -o producer_consumer_2.o; ./producer_consumer_2.o
/*
/ Producer consumer problem
1. Classic problem of synchronization
2. Producers must wait for empty space in buffer
3. Consumers must wait for available data in buffer
*/

#define BUF_SIZE 5 // Size of the buffer

sem_t empty;
sem_t full;
int in = 0;
int out = 0;
int buffer[BUF_SIZE];
pthread_mutex_t mutex;

void *producer(void *pno)
{
    int item;
    while (1)
    {
        item = rand() % 10; // Produce an random item
        printf("                       Producer %d waiting \n", *((int *)pno));
        // Entry section
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        // Critical section
        buffer[in] = item;
        printf("                       Producer %d critical section \nInsert:%d  Pos:%d\n", *((int *)pno), buffer[in], in);
        in = (in + 1) % BUF_SIZE;
        int iter;
        if (in == out)
        {
            printf("%d ", buffer[out]);
            iter = (out + 1) % BUF_SIZE;
        }
        else
            iter = out;
        while (iter != in)
        {
            printf("%d ", buffer[iter]);
            iter = (iter + 1) % BUF_SIZE;
        }
        printf("\n\n");
        // Exit section
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        // Remainder section
        sleep(rand() % 5);
    }
}
void *consumer(void *cno)
{
    while (1)
    {
        // Entry section
        sem_wait(&full);
        printf("                       Consumer %d waiting \n", *((int *)cno));
        pthread_mutex_lock(&mutex);
        // Critical section
        int item = buffer[out];
        printf("                       Consumer %d critical section \nRemove:%d  Pos:%d\n", *((int *)cno), item, out);
        out = (out + 1) % BUF_SIZE;
        // Exit section
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        // Remainder section
        sleep(rand() % 10);
    }
}

int main()
{

    int producers = 1, consumers = 1;
    printf("Number of producers: ");
    scanf("%d", &producers);
    printf("Number of Consumers: ");
    scanf("%d", &consumers);
    pthread_t pro[producers], con[consumers];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUF_SIZE);
    sem_init(&full, 0, 0);

    int a[(producers > consumers ? producers : consumers)];
    for (int i = 0; i < (producers > consumers ? producers : consumers); i++)
        a[i] = i + 1;

    for (int i = 0; i < producers; i++)
    {
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);
    }
    for (int i = 0; i < consumers; i++)
    {
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);
    }

    for (int i = 0; i < producers; i++)
    {
        pthread_join(pro[i], NULL);
    }
    for (int i = 0; i < consumers; i++)
    {
        pthread_join(con[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
