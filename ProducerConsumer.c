#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5

//counting semaphore
sem_t full;
sem_t empty;

//binary semaphore
sem_t S;

int buffer[N];
int in = 0;
int out = 0;

void *ProduceItem(void *ptr)
{
    int item = rand() % 9;

    sem_wait(&empty);
    sem_wait(&S);

    //critical section
    buffer[in] = item;
    printf("Producer: %d producing item: %d\n", *(int *)ptr, buffer[in]);
    in = (in + 1) % N;

    sleep(2);

    sem_post(&S);
    sem_post(&full);

    return 0;
}

void *ConsumeItem(void *ptr)
{
    sem_wait(&full);
    sem_wait(&S);

    //critical section
    int item = buffer[out];
    printf("Consumer: %d producing item: %d\n", *(int *)ptr, item);
    out = (out + 1) % N;

    sleep(2);

    sem_post(&S);
    sem_post(&empty);
    return 0;
}

int main()
{
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, N);
    sem_init(&S, 0, 1);

    int n[N] = {1, 2, 3, 4, 5};

    pthread_t P[N];
    pthread_t C[N];

    for (int i = 0; i < N; i++)
    {
        if (pthread_create(&P[i], NULL, &ProduceItem, (void *)&n[i]) != 0)
        {
            printf("Thread not created.\n");
            exit(-1);
        }
    }

    for (int i = 0; i < N; i++)
    {
        if (pthread_create(&C[i], NULL, &ConsumeItem, (void *)&n[i]) != 0)
        {
            printf("Thread not created.\n");
            exit(-1);
        }
    }

    for (int i = 0; i < N; i++)
    {
        if (pthread_join(P[i], NULL) != 0)
        {
            printf("Thread not joined.\n");
            exit(-1);
        }
    }

    for (int i = 0; i < N; i++)
    {
        if (pthread_join(C[i], NULL) != 0)
        {
            printf("Thread not created.\n");
            exit(-1);
        }
    }

    //destroying semaphore
    sem_destroy(&full);
    sem_destroy(&empty);
    sem_destroy(&S);
    return 0;
}