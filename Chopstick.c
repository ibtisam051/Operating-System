#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define N 5

//binary semaphores
sem_t chopsticks[5];
sem_t S;

//counting semaphore
sem_t count;

//solution 0
/*void *DinPhilospher(void *ptr)
{
    int Pno = *(int *)ptr;
    printf("Philospher: %d is thinking to eat.\n", Pno);

    sem_wait(&chopsticks[Pno]);
    sem_wait(&chopsticks[(Pno + 1) % N]);

    printf("Philospher: %d is eating.\n", Pno);

    sem_post(&chopsticks[Pno]);
    sem_post(&chopsticks[(Pno + 1) % N]);

    return 0;
}*/

//solution 1
/*void *DinPhilospher(void *ptr)
{
    int Pno = *(int *)ptr;

    sem_wait(&S);
    printf("Philospher: %d is thinking to eat.\n", Pno);

    sem_wait(&chopsticks[Pno]);
    sem_wait(&chopsticks[(Pno + 1) % N]);

    printf("Philospher: %d is eating.\n", Pno);
    sem_post(&S);

    sem_post(&chopsticks[Pno]);
    sem_post(&chopsticks[(Pno + 1) % N]);

    return 0;
}*/

//solution 2
/*void *DinPhilospher(void *ptr)
{
    int Pno = *(int *)ptr;

    sem_wait(&count);
    printf("Philospher: %d is thinking to eat.\n", Pno);

    sem_wait(&chopsticks[Pno]);
    sem_wait(&chopsticks[(Pno + 1) % N]);

    printf("Philospher: %d is eating.\n", Pno);

    sem_post(&chopsticks[Pno]);
    sem_post(&chopsticks[(Pno + 1) % N]);

    sem_post(&count);

    return 0;
}*/

//solution 3
void *DinPhilospher(void *ptr)
{
    int Pno = *(int *)ptr;

    printf("Philospher: %d is thinking to eat.\n", Pno);

    if (Pno % 2 == 0)
    {

        sem_wait(&chopsticks[Pno]);
        sem_wait(&chopsticks[(Pno + 1) % N]);
    }
    else
    {
        sem_wait(&chopsticks[(Pno + 1) % N]);
        sem_wait(&chopsticks[Pno]);
    }

    printf("Philospher: %d is eating.\n", Pno);

    sem_post(&chopsticks[Pno]);
    sem_post(&chopsticks[(Pno + 1) % N]);

    return 0;
}

int main()
{
    sem_init(&S, 0, 1);
    sem_init(&count, 0, 5);

    for (int i = 0; i < N; i++)
    {
        sem_init(&chopsticks[i], 0, 1);
    }

    int n[N] = {0, 1, 2, 3, 4};

    pthread_t P[N];
    for (int i = 0; i < N; i++)
    {
        if (pthread_create(&P[i], NULL, &DinPhilospher, (void *)&n[i]) != 0)
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

    //destroying semaphore
    sem_destroy(&S);
    sem_destroy(&count);
    for (int i = 0; i < N; i++)
    {
        sem_destroy(&chopsticks[i]);
    }

    return 0;
}