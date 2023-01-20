#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/wait.h>

long int noOfThreads = 0;
pthread_mutex_t mutex;

void *sorting(void *ptr)
{
    pthread_mutex_lock(&mutex);

    int *arr = (void *)ptr;
    int temp = 0;
    for (int i = 0; i < 10000 / noOfThreads; i++)
    {
        for (int j = i + 1; j < 10000 / noOfThreads; j++)
        {
            if (arr[i] > arr[j])
            {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    printf("\nArray sorted: ");
    for (int i = 0; i < 10000 / noOfThreads; i++)
    {
        printf("%d ", arr[i]);
    }

    pthread_mutex_unlock(&mutex);

    return 0;
}

int main(int argc, char **argv)
{
    if (argc - 1 == 0)
    {
        printf("No input.\n");
    }
    else
    {
        noOfThreads = atoi(argv[1]);
        printf("No of Threads are: %ld\n", noOfThreads);
        pthread_mutex_init(&mutex, NULL); // for synchronization

        //INITIALIZING THE ARRAY USING RAND()
        int *ptr = (int *)malloc((10000) * sizeof(int));
        int lower = 1, upper = 32;
        for (int i = 0; i < 10000; i++)
        {
            ptr[i] = (rand() % (upper - lower - 1)) + lower;
        }

        printf("\nArray after initializing: ");
        for (int i = 0; i < 10000; i++)
        {
            printf("%d ", ptr[i]);
        }
        printf("\n");

        if (10000 % noOfThreads == 0)
        {

            pthread_t threads[noOfThreads]; // let's suppose threads are 5 due to 20/4 = 5
            int x = 0;                      // counter update for array;

            for (int i = 0; i < noOfThreads; i++)
            {
                int *arr = (int *)malloc((10000 / noOfThreads) * sizeof(int)); // 5 size array
                for (int i = 0; i < 10000 / noOfThreads; i++)                  // loop i < 5
                {
                    arr[i] = ptr[x];
                    x++;
                }
                if (pthread_create(&threads[i], NULL, &sorting, (void *)arr) != 0)
                {
                    exit(-1);
                    printf("Thread not created.\n");
                }
            }

            for (int i = 0; i < noOfThreads; i++)
            {
                if (pthread_join(threads[i], NULL) != 0)
                {
                    exit(-1);
                    printf("Thread not joined.\n");
                }
            }

            printf("\n");

            pthread_mutex_destroy(&mutex);
        }
        else
        {
            printf("Not evenly divisible.\n");
        }
    }
    return 0;
}