#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/wait.h>

long int size = 10000;
int extraArr[10000];
int noOfThreads = 0;
int remainingSizeofArray = 0;
int thd = 0;

pthread_mutex_t mutex;

void *sorting(void *ptr)
{
    pthread_mutex_lock(&mutex);

    int *arr = (void *)ptr;
    int temp = 0;
    for (int i = 0; i < size / noOfThreads; i++)
    {
        for (int j = i + 1; j < size / noOfThreads; j++)
        {
            if (arr[i] > arr[j])
            {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    printf("\nArray sorted for thread %d: ", thd);
    for (int i = 0; i < size / noOfThreads; i++)
    {
        printf("%d ", arr[i]);
    }

    thd++;

    pthread_mutex_unlock(&mutex);

    return (void *)arr;
}

void *Oddsorting(void *ptr)
{
    pthread_mutex_lock(&mutex);

    int *arr = (void *)ptr;
    int temp = 0;
    for (int i = 0; i < remainingSizeofArray; i++)
    {
        for (int j = i + 1; j < remainingSizeofArray; j++)
        {
            if (arr[i] > arr[j])
            {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    printf("\nArray sorted for thread %d: ", thd);
    for (int i = 0; i < remainingSizeofArray; i++)
    {
        printf("%d ", arr[i]);
    }

    thd++;

    pthread_mutex_unlock(&mutex);

    return (void *)arr;
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
        printf("No of Threads are: %d\n", noOfThreads);
        pthread_mutex_init(&mutex, NULL); // for synchronization

        //INITIALIZING THE ARRAY USING RAND()
        int *ptr = (int *)malloc((size) * sizeof(int));
        int lower = 1, upper = 32;
        for (int i = 0; i < size; i++)
        {
            ptr[i] = (rand() % (upper - lower - 1)) + lower;
        }

        printf("\nArray after initializing: ");
        for (int i = 0; i < size; i++)
        {
            printf("%d ", ptr[i]);
        }
        printf("\n");

        if (size % noOfThreads == 0)
        {
            pthread_t threads[noOfThreads]; // 4
            int x = 0;                      //for array update counter

            for (int i = 0; i < noOfThreads; i++)
            {
                int *temp = (int *)malloc((size / noOfThreads) * sizeof(int)); // 20/4 = 5 size array
                for (int j = 0; j < size / noOfThreads; j++)
                {
                    temp[j] = ptr[x]; //0,1,2,3,4 - 5,6,7,8,9 -
                    x++;
                }
                if (pthread_create(&threads[i], NULL, &sorting, (void *)temp) != 0)
                {
                    exit(-1);
                    printf("Thread not created.\n");
                }
            }

            void *returnArr;
            int *returnArr2;
            int y = 0;

            for (int i = 0; i < noOfThreads; i++)
            {
                if (pthread_join(threads[i], &returnArr) != 0)
                {
                    exit(-1);
                    printf("Thread not created.\n");
                }

                returnArr2 = (int *)returnArr;

                for (int j = 0; j < size / noOfThreads; j++)
                {
                    extraArr[y] = returnArr2[j];
                    y++;
                }
            }

            printf("\n\nBefore extra Array: ");
            for (int i = 0; i < size; i++)
            {
                printf("%d ", extraArr[i]);
            }

            int blue = 0;
            for (int i = 0; i < size; i++)
            {
                for (int j = i + 1; j < size; j++)
                {
                    if (extraArr[i] > extraArr[j])
                    {
                        blue = extraArr[i];
                        extraArr[i] = extraArr[j];
                        extraArr[j] = blue;
                    }
                }
            }

            printf("\n\nAfter extra Array: ");
            for (int i = 0; i < size; i++)
            {
                printf("%d ", extraArr[i]);
            }
            printf("\n");
        }
        else
        {
            pthread_t threads2[noOfThreads]; // 6
            int x = 0;                       //for array update counter

            for (int i = 0; i < noOfThreads; i++) // 6 times 0,1,2,3,4,5
            {
                if (i != noOfThreads - 1) // 0,1,2,3,4
                {
                    int *temp = (int *)malloc((size / noOfThreads) * sizeof(int)); // 20/6 = 3 size array
                    for (int j = 0; j < size / noOfThreads; j++)
                    {
                        temp[j] = ptr[x];
                        x++;
                    }
                    if (pthread_create(&threads2[i], NULL, &sorting, (void *)temp) != 0)
                    {
                        exit(-1);
                        printf("Thread not created.\n");
                    }
                }
                else if (i == noOfThreads - 1) // 5
                {
                    //printf("\nX:%d\n", x);
                    remainingSizeofArray = size - x; //20 - 15 = 5

                    int *temp2 = (int *)malloc((remainingSizeofArray) * sizeof(int)); // 5 = 5 size array
                    for (int j = 0; j < remainingSizeofArray; j++)
                    {
                        temp2[j] = ptr[x];
                        x++;
                    }
                    if (pthread_create(&threads2[i], NULL, &Oddsorting, (void *)temp2) != 0)
                    {
                        exit(-1);
                        printf("Thread not created.\n");
                    }
                }
            }

            void *returnArr;
            int *returnArr2;
            int y = 0;

            for (int i = 0; i < noOfThreads; i++)
            {
                if (pthread_join(threads2[i], &returnArr) != 0)
                {
                    exit(-1);
                    printf("Thread not created.\n");
                }

                returnArr2 = (int *)returnArr;

                if (i != noOfThreads - 1)
                {
                    for (int j = 0; j < size / noOfThreads; j++)
                    {
                        extraArr[y] = returnArr2[j];
                        y++;
                    }
                }
                else if (i == noOfThreads - 1)
                {
                    for (int j = 0; j < remainingSizeofArray; j++)
                    {
                        extraArr[y] = returnArr2[j];
                        y++;
                    }
                }
            }

            printf("\n\nBefore extra Array: ");
            for (int i = 0; i < size; i++)
            {
                printf("%d ", extraArr[i]);
            }

            int blue = 0;
            for (int i = 0; i < size; i++)
            {
                for (int j = i + 1; j < size; j++)
                {
                    if (extraArr[i] > extraArr[j])
                    {
                        blue = extraArr[i];
                        extraArr[i] = extraArr[j];
                        extraArr[j] = blue;
                    }
                }
            }

            printf("\n\nAfter extra Array: ");
            for (int i = 0; i < size; i++)
            {
                printf("%d ", extraArr[i]);
            }
            printf("\n");
        }

        pthread_mutex_destroy(&mutex);
    }
    return 0;
}