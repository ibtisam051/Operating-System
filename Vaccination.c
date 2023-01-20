#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5

//binary semaphores
sem_t t;
sem_t s;

//counting semaphores
sem_t t3;
sem_t s3;

int sCount = 0;
int tCount = 0;

void *VaccinationT(void *ptr)
{
    int Tno = *(int *)ptr;

    sem_wait(&t3);

    sem_wait(&t);
    tCount += 1;
    if (tCount == 1)
    {
        sem_wait(&s);
    }
    sem_post(&t);

    //critical section
    printf("Teacher: %d is vaccinating.\n", Tno);
    sleep(2);

    sem_wait(&t);
    tCount -= 1;
    if (tCount == 0)
    {
        sem_post(&s);
    }
    sem_post(&t);

    sem_post(&t3);
    return 0;
}

void *VaccinationS(void *ptr)
{
    int Tno = *(int *)ptr;

    sem_wait(&s3);

    sem_wait(&s);
    sCount += 1;
    if (sCount == 1)
    {
        sem_wait(&t);
    }
    sem_post(&s);

    //critical section
    printf("Student: %d is vaccinating.\n", Tno);
    sleep(2);

    sem_wait(&s);
    sCount -= 1;
    if (sCount == 0)
    {
        sem_post(&t);
    }
    sem_post(&s);

    sem_post(&s3);
    return 0;
}

int main()
{
    sem_init(&t, 0, 1);
    sem_init(&s, 0, 1);
    sem_init(&t3, 0, 3);
    sem_init(&s3, 0, 3);

    int n[N] = {1, 2, 3, 4, 5};

    pthread_t Tthd[N];
    pthread_t Sthd[N];

    for (int i = 0; i < N; i++)
    {
        if (pthread_create(&Tthd[i], NULL, &VaccinationT, (void *)&n[i]) != 0)
        {
            printf("Error.\n");
            exit(-1);
        }
    }

    for (int i = 0; i < N; i++)
    {
        if (pthread_create(&Sthd[i], NULL, &VaccinationS, (void *)&n[i]) != 0)
        {
            printf("Error.\n");
            exit(-1);
        }
    }

    for (int i = 0; i < N; i++)
    {
        if (pthread_join(Tthd[i], NULL) != 0)
        {
            printf("Error.\n");
            exit(-1);
        }
    }

    for (int i = 0; i < N; i++)
    {
        if (pthread_join(Sthd[i], NULL) != 0)
        {
            printf("Error.\n");
            exit(-1);
        }
    }

    //destroying semaphores
    sem_destroy(&t);
    sem_destroy(&s);
    sem_destroy(&t3);
    sem_destroy(&s3);
    return 0;
}