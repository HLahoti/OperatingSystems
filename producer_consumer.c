#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define N 15

// gcc -g -pthread producer_consumer.c -o producer_consumer.o
/*
/ Producer consumer problem
1. Classic problem of synchronization
2. Producer must wait for empty space in buffer
3. Consumer must wait for available data in buffer
4. Here it is solved using 1 binary semaphore and 2 counting semaphores
*/

int max_buf = N;
int buffer[N];
// semaphores
int upd_buf;
int avail;
int emp;

struct
{
    float wait_time;
} typedef th_info;

void wait_semaphore(int *semaphore)
{
    while (*semaphore <= 0)
        ;
    *semaphore = *semaphore - 1;
}

void signal_semaphore(int *semaphore)
{
    *semaphore = *semaphore + 1;
}

void *producer(void *arg)
{
    th_info *params = (th_info *)arg;
    int i;
    do
    {
        printf("                     P Waiting\n");
        wait_semaphore(&emp);
        wait_semaphore(&upd_buf);

        // Critical section
        printf("                     P CS\n");
        int x = rand() % 10;
        buffer[max_buf - emp - 1] = x;
        printf("Putting %d in buffer.\n", x);
        for (i = 0; i <= avail; i++)
            printf("%d ", buffer[i]);
        printf("\n-----------------\n");
        // End of critical section

        signal_semaphore(&avail);
        signal_semaphore(&upd_buf);
        // printf("%d %d\n", emp, avail);
        sleep(params->wait_time);
    } while (1);
}

void *consumer(void *arg)
{
    th_info *params = (th_info *)arg;
    do
    {
        printf("                     C Waiting\n");
        wait_semaphore(&avail);
        wait_semaphore(&upd_buf);

        // Critical section
        printf("                     C CS\n");
        int y = buffer[avail];
        printf("Read %d from buffer.\n-----------------\n", y);
        // End of critical section

        signal_semaphore(&emp);
        signal_semaphore(&upd_buf);
        // printf("%d %d\n", emp, avail);
        sleep(params->wait_time);
    } while (1);
}

int main()
{
    srand(time(NULL));
    th_info consumer_info, producer_info;

    printf("Wait time for consumer: ");
    scanf("%e", &consumer_info.wait_time);
    printf("Wait time for producer: ");
    scanf("%e", &producer_info.wait_time);
    printf("Buffer size (max 15): ");
    scanf("%d", &max_buf);
    upd_buf = 1;
    avail = 0;
    emp = max_buf - avail;

    pthread_t *consumer_thread = (pthread_t *)malloc(sizeof(pthread_t));
    pthread_t *producer_thread = (pthread_t *)malloc(sizeof(pthread_t));
    pthread_create(consumer_thread, NULL, &consumer, &consumer_info);
    pthread_create(producer_thread, NULL, &producer, &producer_info);

    pthread_join(*consumer_thread, NULL);
    pthread_join(*producer_thread, NULL);
    return 0;
}
