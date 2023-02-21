#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

pthread_mutex_t mutex;
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

    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);

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

    // Destroy mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}

void* tobaccoSmoker(void* arg) {
    while (true) {
        pthread_mutex_lock(&mutex);
        if (hasPaper && hasMatches) {
            hasPaper = false;
            hasMatches = false;
            printf("Tobacco smoker is smoking\n");
            putIngredientsOnTable();
            smoke();
        }
        pthread_mutex_unlock(&mutex);
    }
}

void* paperSmoker(void* arg) {
    while (true) {
        pthread_mutex_lock(&mutex);
        if (hasTobacco && hasMatches) {
            hasTobacco = false;
            hasMatches = false;
            printf("Paper smoker is smoking\n");
            putIngredientsOnTable();
            smoke();
        }
        pthread_mutex_unlock(&mutex);
    }
}

void* matchSmoker(void* arg) {
    while (true) {
        pthread_mutex_lock(&mutex);
        if (hasTobacco && hasPaper) {
            hasTobacco = false;
            hasPaper = false;
            printf("Match smoker is smoking\n");
            putIngredientsOnTable();
            smoke();
        }
        pthread_mutex_unlock(&mutex);
    }
}

void* agent(void* arg) {
    while (true) {
        pthread_mutex_lock(&mutex);
        putIngredientsOnTable();
        if (missingIngredient == 0) {
            hasPaper = true;
            hasMatches = true;
        } else if (missingIngredient == 1) {
            hasTobacco = true;
            hasMatches = true;
        } else {
            hasTobacco = true;
            hasPaper = true;
        }
        pthread_mutex_unlock(&mutex);
    }
}

void putIngredientsOnTable() {
    int r = rand() % 3;
    missingIngredient = r;
    if (r == 0) {
        printf("Agent put paper and matches on the table\n");
    } else if (r == 1) {
        printf("Agent put tobacco and matches on the table\n");
    } else {
        printf("Agent put tobacco and paper on the table\n");
    }
}

void smoke() {
    // Simulate smoking by sleeping for a random amount of time
    int sleepTime = rand() % 1000 + 1;
    usleep(sleepTime * 1000);
}
