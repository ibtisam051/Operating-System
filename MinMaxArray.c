#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>

int globalSize = 0;

void *MinimumOfArray(void *ptr)
{
    int *array = (int *)ptr;
    int *min = (int *)malloc(sizeof(int));
    *min = array[0];
    for (int i = 1; i < globalSize; i++)
    {
        if (array[i] < *min)
        {
            *min = array[i];
        }
    }
    //printf("The minimum value is: %d\n", min);
    return (void *)min;
}

void *MaxofArray(void *ptr)
{
    int *array = (int *)ptr;
    int *max = (int *)malloc(sizeof(int));
    *max = array[0];
    for (int i = 1; i < globalSize; i++)
    {
        if (array[i] > *max)
        {
            *max = array[i];
        }
    }

    //printf("The minimum value is: %d\n", max);
    return (void *)max;
}

void *AvgOfArray(void *ptr)
{
    float *result = (float *)malloc(sizeof(float));
    float sum = 0;
    int *array = (int *)ptr;
    for (int i = 0; i < globalSize; i++)
    {
        sum += array[i];
    }

    *result = sum / globalSize;
    // printf("The average value is: %f\n", result);
    return (void *)result;
}

int main(int argc, char **argv)
{
    globalSize = argc - 1;

    if (globalSize == 0)
    {
        printf("No Input.\n");
    }
    else
    {
        int *array = (int *)malloc((globalSize) * sizeof(int));
        for (int i = 0; i < globalSize; i++)
        {
            array[i] = atoi(argv[i + 1]);
        }

        pthread_t thd1, thd2, thd3;
        pthread_create(&thd1, NULL, &AvgOfArray, (void *)array);
        pthread_create(&thd2, NULL, &MinimumOfArray, (void *)array);
        pthread_create(&thd3, NULL, &MaxofArray, (void *)array);

        void *retVal1, *retVal2, *retVal3;
        pthread_join(thd1, &retVal1);
        pthread_join(thd2, &retVal2);
        pthread_join(thd3, &retVal3);

        float average = *((float *)retVal1);
        printf("Average of array is: %f\n", average);

        int min = *((int *)retVal2);
        printf("Min in Array is: %d\n", min);

        int max = *((int *)retVal3);
        printf("Max in Array is: %d\n", max);

        free(array);
    }
    return 0;
}