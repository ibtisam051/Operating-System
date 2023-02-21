#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <time.h>

sem_t tobaccoSem;
sem_t paperSem;
sem_t matchSem;
sem_t agentSem;
sem_t mutex;

bool hasTobacco = false;
bool hasPaper = false;
bool hasMatches = false;
int missingIngredient = 0;

void* tobaccoSmoker(void* arg);
void* paperSmoker(void* arg);
void* matchSmoker(void* arg);
void* agent(void* arg);

void putIngredientsOnTable();
void smoke();

int main() {
    srand(time(NULL));

    // Initialize semaphores
    sem_init(&tobaccoSem, 0, 0);
    sem_init(&paperSem, 0, 0);
    sem_init(&matchSem, 0, 0);
    sem_init(&agentSem, 0, 1);
    sem_init(&mutex, 0, 1);

    // Create threads for each smoker and the agent
    pthread_t t1, t2, t3, a;
    pthread_create(&t1, NULL, tobaccoSmoker, NULL);
    pthread_create(&t2, NULL, paperSmoker, NULL);
    pthread_create(&t3, NULL, matchSmoker, NULL);
    pthread_create(&a, NULL, agent, NULL);

    // Wait for threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(a, NULL);

    // Destroy semaphores
    sem_destroy(&tobaccoSem);
    sem_destroy(&paperSem);
    sem_destroy(&matchSem);
    sem_destroy(&agentSem);
    sem_destroy(&mutex);

    return 0;
}

void* tobaccoSmoker(void* arg) {
    while (true) {
        sem_wait(&mutex);
        if (hasPaper && hasMatches) {
            hasPaper = false;
            hasMatches = false;
            printf("Tobacco smoker is smoking\n");
            sem_post(&agentSem);
            smoke();
        }
        sem_post(&mutex);
        sem_wait(&tobaccoSem);
    }
}

void* paperSmoker(void* arg) {
    while (true) {
        sem_wait(&mutex);
        if (hasTobacco && hasMatches) {
            hasTobacco = false;
            hasMatches = false;
            printf("Paper smoker is smoking\n");
            sem_post(&agentSem);
            smoke();
        }
        sem_post(&mutex);
        sem_wait(&paperSem);
    }
}

void* matchSmoker(void* arg) {
    while (true) {
        sem_wait(&mutex);
        if (hasTobacco && hasPaper) {
            hasTobacco = false;
            hasPaper = false;
            printf("Match smoker is smoking\n");
            sem_post(&agentSem);
            smoke();
        }
        sem_post(&mutex);
        sem_wait(&matchSem);
    }
}

void* agent(void* arg) {
    while (true) {
        sem_wait(&mutex);
        putIngredientsOnTable();
        if (missingIngredient == 0) {
            sem_post(&tobaccoSem);
        } else if (missingIngredient == 1) {
            sem_post(&paperSem);
        } else {
            sem_post(&matchSem);
        }
        sem_wait(&agentSem);
        sem_post(&mutex);
    }
}

void putIngredientsOnTable() {
    int r = rand() % 3;
    missingIngredient = r;
    if (r == 0) {
        hasPaper = true;
        hasMatches = true;
        printf("Agent put paper and matches on the table\n");
    } else if (r == 1) {
        hasTobacco = true;
        hasMatches = true;
        printf("Agent put tobacco and matches on the table\n");
    } else {
        hasTobacco = true;
        hasPaper = true;
        printf("Agent put tobacco and paper on the table\n");
    }
}
