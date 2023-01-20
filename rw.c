#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

//binary semaphore
sem_t mutex;
sem_t db;

//counting semaphore
sem_t r5;
sem_t w1;

int rc = 0;

void *Reader(void *ptr)
{
    sem_wait(&r5); //5-.>4

    sem_wait(&mutex);
    rc += 1;
    if (rc == 1)
    {
        sem_wait(&db); //stoping writer
    }
    sem_post(&mutex);

    printf("Reader: %d is reading.\n", *(int *)ptr);
    sleep(2);

    sem_wait(&mutex);
    rc -= 1;
    if (rc == 0)
    {
        sem_post(&db);

        sem_post(&r5);
        sem_post(&r5);
        sem_post(&r5);
        sem_post(&r5);
        sem_post(&r5);
    }
    sem_post(&mutex);

    return 0;
}

void *Writer(void *ptr)
{
    sem_wait(&w1);

    sem_wait(&db);

    printf("Writer: %d is writing.\n", *(int *)ptr);
    sleep(2);

    sem_post(&db);

    sem_post(&w1);
    return 0;
}

int main()
{
    sem_init(&mutex, 0, 1);
    sem_init(&db, 0, 1);
    sem_init(&w1, 0, 1);
    sem_init(&r5, 0, 5);

    int n[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    pthread_t R[8];
    pthread_t W[3];

    for (int i = 0; i < 8; i++)
    {
        if (pthread_create(&R[i], NULL, &Reader, (void *)&n[i]) != 0)
        {
            printf("Thread not created.\n");
            exit(-1);
        }
    }

    for (int i = 0; i < 3; i++)
    {
        if (pthread_create(&W[i], NULL, &Writer, (void *)&n[i]) != 0)
        {
            printf("Thread not created.\n");
            exit(-1);
        }
    }

    for (int i = 0; i < 8; i++)
    {
        if (pthread_join(R[i], NULL) != 0)
        {
            printf("Thread not created.\n");
            exit(-1);
        }
    }

    for (int i = 0; i < 3; i++)
    {
        if (pthread_join(W[i], NULL) != 0)
        {
            printf("Thread not created.\n");
            exit(-1);
        }
    }

    //destroying semaphore
    sem_destroy(&mutex);
    sem_destroy(&db);
    sem_destroy(&r5);
    sem_destroy(&w1);
    return 0;
}