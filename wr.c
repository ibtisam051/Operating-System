#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MaxItems 8 // maximum readers or writers

int rc = 0;

//binary semaphore
sem_t mutex;
sem_t db;

void *writer(void *ptr)
{
    sem_wait(&mutex);
    rc = rc + 1;
    if (rc == 1)
    {
        sem_wait(&db);
    }
    sem_post(&mutex);

    // critical section
    printf("I'm Writer: %d and i'm writing.\n", *(int *)ptr);

    // exit section
    sem_wait(&mutex);
    rc = rc - 1;
    if (rc == 0)
    {
        sem_post(&db);
    }
    sem_post(&mutex);
    return 0;
}

void *reader(void *ptr)
{
    // entry section
    sem_wait(&db);

    // critical section
    printf("I'm Reader: %d and i'm reading.\n", *(int *)ptr);

    // exit section
    sem_post(&db);
    return 0;
}

int main()
{
    pthread_t wr[MaxItems]; //writer threads
    pthread_t rd[MaxItems]; //reader threads

    sem_init(&mutex, 0, 1);
    sem_init(&db, 0, 1);

    int n[MaxItems] = {1, 2, 3, 4, 5, 6, 7, 8}; // just to check which writer or reader is working

    //THREADS CREATION
    for (int i = 0; i < MaxItems; i++)
    {
        if (pthread_create(&wr[i], NULL, &writer, (void *)&n[i]) != 0)
        {
            printf("Thread %d not created.\n", i);
            exit(-1);
        }
    }

    for (int i = 0; i < MaxItems; i++)
    {
        if (pthread_create(&rd[i], NULL, &reader, (void *)&n[i]) != 0)
        {
            printf("Thread %d not created.\n", i);
            exit(-1);
        }
    }

    //THREADS JOINING
    for (int i = 0; i < MaxItems; i++)
    {
        if (pthread_join(wr[i], NULL) != 0)
        {
            printf("Thread %d not joined.\n", i);
            exit(-1);
        }
    }

    for (int i = 0; i < MaxItems; i++)
    {
        if (pthread_join(rd[i], NULL) != 0)
        {
            printf("Thread %d not joined.\n", i);
            exit(-1);
        }
    }

    //DESTROYING SEMAPHORES
    sem_destroy(&mutex);
    sem_destroy(&db);
    return 0;
}