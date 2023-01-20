#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct Data
{
    int *ptr;
    int size;
};

void *Average(void *ptr)
{
    struct Data Obj2 = *(struct Data *)ptr;

    float *avg = (float *)malloc(sizeof(float));
    int sum = 0;

    for (int i = 0; i < Obj2.size; i++)
    {
        sum += Obj2.ptr[i];
    }

    *avg = sum / Obj2.size;

    return (void *)avg;
}

void *Minimum(void *ptr)
{
    struct Data Obj2 = *(struct Data *)ptr;

    int *min = (int *)malloc(sizeof(int));
    *min = Obj2.ptr[0];

    for (int i = 1; i < Obj2.size; i++)
    {
        if (Obj2.ptr[i] < *min)
        {
            *min = Obj2.ptr[i];
        }
    }

    return (void *)min;
}

void *Maximum(void *ptr)
{
    struct Data Obj2 = *(struct Data *)ptr;

    int *max = (int *)malloc(sizeof(int));
    *max = Obj2.ptr[0];

    for (int i = 1; i < Obj2.size; i++)
    {
        if (Obj2.ptr[i] > *max)
        {
            *max = Obj2.ptr[i];
        }
    }

    return (void *)max;
}

int main(int argc, char **argv)
{
    if (argc - 1 == 0)
    {
        printf("No Input.\n");
    }
    else
    {
        struct Data Obj;
        Obj.size = argc - 1;
        Obj.ptr = (int *)malloc(Obj.size * sizeof(int));

        for (int i = 0; i < Obj.size; i++)
        {
            Obj.ptr[i] = atoi(argv[i + 1]);
        }

        printf("Array is: ");
        for (int i = 0; i < Obj.size; i++)
        {
            //Obj.ptr[i] = atoi(argv[i + 1]);
            printf("%d ", Obj.ptr[i]);
        }

        pthread_t threads[3];

        //creation
        for (int i = 0; i < 3; i++)
        {
            if (i == 0)
            {
                if (pthread_create(&threads[i], NULL, &Average, (void *)&Obj) != 0)
                {
                    printf("Thread not created.\n");
                    exit(-1);
                }
            }
            else if (i == 1)
            {
                if (pthread_create(&threads[i], NULL, &Minimum, (void *)&Obj) != 0)
                {
                    printf("Thread not created.\n");
                    exit(-1);
                }
            }
            else if (i == 2)
            {
                if (pthread_create(&threads[i], NULL, &Maximum, (void *)&Obj) != 0)
                {
                    printf("Thread not created.\n");
                    exit(-1);
                }
            }
        }

        //joining

        void *ret1, *ret2, *ret3;
        for (int i = 0; i < 3; i++)
        {
            if (i == 0)
            {
                if (pthread_join(threads[i], &ret1) != 0)
                {
                    printf("Thread not joined.\n");
                    exit(-1);
                }
            }
            else if (i == 1)
            {
                if (pthread_join(threads[i], &ret2) != 0)
                {
                    printf("Thread not created.\n");
                    exit(-1);
                }
            }
            else if (i == 2)
            {
                if (pthread_join(threads[i], &ret3) != 0)
                {
                    printf("Thread not created.\n");
                    exit(-1);
                }
            }
        }

        float avg = *(float *)ret1;
        int min = *(int *)ret2;
        int max = *(int *)ret3;

        printf("\nAverage is: %f\n", avg);
        printf("Min is: %d\n", min);
        printf("Max is: %d\n", max);
    }
    return 0;
}