#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_AGENTS 3
#define NUM_SMOKERS 3

pthread_mutex_t mutex;
pthread_cond_t agent_cv, smoker_cv;

int ingredients = 0;

void* agent(void* arg) {
    int my_ingredient;
    while(1) {
        pthread_mutex_lock(&mutex);
        while(ingredients != 0) {
            pthread_cond_wait(&agent_cv, &mutex);
        }
        my_ingredient = rand() % 3 + 1;
        printf("Agent %d produced ingredient %d.\n", *((int*)arg), my_ingredient);
        ingredients = my_ingredient;
        pthread_cond_signal(&smoker_cv);
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 3 + 1);
    }
    pthread_exit(NULL);
}

void* smoker(void* arg) {
    int my_ingredient = *((int*)arg);
    while(1) {
        pthread_mutex_lock(&mutex);
        while(ingredients != my_ingredient) {
            pthread_cond_wait(&smoker_cv, &mutex);
        }
        printf("Smoker %d is smoking.\n", my_ingredient);
        ingredients = 0;
        pthread_cond_signal(&agent_cv);
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 3 + 1);
    }
    pthread_exit(NULL);
}

int main() {
    int i;
    int agent_ids[NUM_AGENTS];
    int smoker_ids[NUM_SMOKERS];
    pthread_t agents[NUM_AGENTS];
    pthread_t smokers[NUM_SMOKERS];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&agent_cv, NULL);
    pthread_cond_init(&smoker_cv, NULL);

    for(i = 0; i < NUM_AGENTS; i++) {
        agent_ids[i] = i + 1;
        pthread_create(&agents[i], NULL, agent, (void*) &agent_ids[i]);
    }

    for(i = 0; i < NUM_SMOKERS; i++) {
        smoker_ids[i] = i + 1;
        pthread_create(&smokers[i], NULL, smoker, (void*) &smoker_ids[i]);
    }

    for(i = 0; i < NUM_AGENTS; i++) {
        pthread_join(agents[i], NULL);
    }

    for(i = 0; i < NUM_SMOKERS; i++) {
        pthread_join(smokers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&agent_cv);
    pthread_cond_destroy(&smoker_cv);

    return 0;
}
