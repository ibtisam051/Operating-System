#include <pthread.h>
#include <stdio.h>

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2

int buffer[BUFFER_SIZE];
int in = 0, out = 0;
int count = 0;
pthread_mutex_t mutex;
pthread_cond_t empty, full;

void *producer(void *arg) {
    int i;
    for (i = 0; i < NUM_PRODUCERS; i++) {
        pthread_mutex_lock(&mutex);
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&empty, &mutex);
        }
        buffer[in] = i;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

void *consumer(void *arg) {
    int i, item;
    for (i = 0; i < NUM_CONSUMERS; i++) {
        pthread_mutex_lock(&mutex);
        while (count == 0) {
            pthread_cond_wait(&full, &mutex);
        }
        item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
        printf("Consumer %d consumed item %d\n", (int)arg, item);
    }
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    int i, producer_args[NUM_PRODUCERS], consumer_args[NUM_CONSUMERS];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&empty, NULL);
    pthread_cond_init(&full, NULL);

    for (i = 0; i < NUM_PRODUCERS; i++) {
        producer_args[i] = i;
        pthread_create(&producers[i], NULL, producer, &producer_args[i]);
    }

    for (i = 0; i < NUM_CONSUMERS; i++) {
        consumer_args[i] = i;
        pthread_create(&consumers[i], NULL, consumer, &consumer_args[i]);
    }

    for (i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }

    for (i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }

    pthread_cond_destroy(&empty);
    pthread_cond_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
