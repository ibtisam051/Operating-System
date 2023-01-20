//sum,max,2max,mode,median

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct Data
{
    int *ptr;
    int size;
};

void *Mode(void *ptr)
{
    struct Data Obj2 = *(struct Data *)ptr;

    float arr[Obj2.size];
    int count = 0;
    for (int i = 0; i < Obj2.size; i++)
    {
        for (int j = 0; j < Obj2.size; j++)
        {
            if (Obj2.ptr[i] == Obj2.ptr[j])
            {
                count++;
            }
        }
        arr[i] = count;
        count = 0;
    }

    float max = arr[0];
    int k = 1;
    int index = 0;
    for (k = 1; k < Obj2.size; k++)
    {
        if (arr[k] > max)
        {
            max = arr[k];
            index = k;
        }
    }

    float *mode = (float *)malloc(sizeof(float));
    *mode = Obj2.ptr[index];

    return (void *)mode;
}

void *Sum(void *ptr)
{
    struct Data Obj2 = *(struct Data *)ptr;
    int *sum = (int *)malloc(sizeof(int));

    for (int i = 0; i < Obj2.size; i++)
    {
        *sum += Obj2.ptr[i];
    }
    return (void *)sum;
}

void *Maximum(void *ptr)
{
    struct Data Obj2 = *(struct Data *)ptr;
    int *max = (int *)malloc(sizeof(int));

    *max = Obj2.ptr[0];

    for (int i = 0; i < Obj2.size; i++)
    {
        if (Obj2.ptr[i] > *max)
        {
            *max = Obj2.ptr[i];
        }
    }
    return (void *)max;
}

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

void *Median(void *ptr)
{
    struct Data Obj2 = *(struct Data *)ptr;
    int *arr = Obj2.ptr;
    int temp = 0;

    for (int i = 0; i < Obj2.size; i++)
    {
        for (int j = i + 1; j < Obj2.size; j++)
        {
            if (arr[i] > arr[j])
            {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }

    float *median = (float *)malloc(sizeof(float));

    for (int i = 0; i < Obj2.size; i++)
    {
        if (i % 2 == 0)
        {
            *median = (arr[(i - 1) / 2] + arr[i / 2]) / 2;
        }
        else
        {
            *median = arr[i / 2];
        }
    }
    return (void *)median;
}

void *SecondMaximum(void *ptr)
{
    struct Data Obj2 = *(struct Data *)ptr;
    int *arr = Obj2.ptr;
    int temp = 0;

    for (int i = 0; i < Obj2.size; i++)
    {
        for (int j = i + 1; j < Obj2.size; j++)
        {
            if (arr[i] < arr[j])
            {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }

    int *max = (int *)malloc(sizeof(int));
    *max = arr[1];
    return (void *)max;
}

void *ThirdMinimum(void *ptr)
{
    struct Data Obj2 = *(struct Data *)ptr;

    int *min = (int *)malloc(sizeof(int));
    *min = Obj2.ptr[0];

    int *smin = (int *)malloc(sizeof(int));
    *smin = Obj2.ptr[1];

    int *tmin = (int *)malloc(sizeof(int));
    *tmin = Obj2.ptr[2];

    for (int i = 2; i < Obj2.size; i++)
    {
        if (Obj2.ptr[i] < *min)
        {
            *tmin = *smin;
            *smin = *min;
            *min = Obj2.ptr[i];
        }
        else if (Obj2.ptr[i] < *smin)
        {
            *tmin = *smin;
            *smin = Obj2.ptr[i];
        }
        else if (Obj2.ptr[i] < *tmin && Obj2.ptr[i] != *smin && Obj2.ptr[i] != *min)
        {
            *tmin = Obj2.ptr[i];
        }
    }

    return (void *)tmin;
}

int main(int argc, char **argv)
{
    if (argc - 1 == 0)
    {
        printf("No Input.\n");
    }
    else
    {
        int N = atoi(argv[1]);

        if (N > 0 && N < 7)
        {

            struct Data Obj;
            Obj.size = argc - 2;

            printf("N is: %d\n", N);
            int x = 0;

            Obj.ptr = (int *)malloc(Obj.size * sizeof(int));
            for (int i = 1; i <= Obj.size; i++)
            {
                Obj.ptr[x] = atoi(argv[i + 1]);
                x++;
            }

            printf("Array is: ");
            for (int i = 0; i < Obj.size; i++)
            {
                printf("%d ", Obj.ptr[i]);
            }

            pthread_t threads[N];

            //thread creation
            for (int i = 0; i <= N; i++)
            {
                if (i == 0)
                {
                    if (pthread_create(&threads[i], NULL, &Mode, (void *)&Obj) != 0)
                    {
                        printf("Thread not created.\n");
                        exit(-1);
                    }
                }
                else if (i == 1)
                {
                    if (pthread_create(&threads[i], NULL, &Sum, (void *)&Obj) != 0)
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
                else if (i == 3)
                {
                    if (pthread_create(&threads[i], NULL, &Average, (void *)&Obj) != 0)
                    {
                        printf("Thread not created.\n");
                        exit(-1);
                    }
                }
                else if (i == 4)
                {
                    if (pthread_create(&threads[i], NULL, &Median, (void *)&Obj) != 0)
                    {
                        printf("Thread not created.\n");
                        exit(-1);
                    }
                }
                else if (i == 5)
                {
                    if (pthread_create(&threads[i], NULL, &SecondMaximum, (void *)&Obj) != 0)
                    {
                        printf("Thread not created.\n");
                        exit(-1);
                    }
                }
                else if (i == 6)
                {
                    if (pthread_create(&threads[i], NULL, &ThirdMinimum, (void *)&Obj) != 0)
                    {
                        printf("Thread not created.\n");
                        exit(-1);
                    }
                }
            }

            //thread joining

            void *ret1, *ret2, *ret3, *ret4, *ret5, *ret6, *ret7;
            for (int i = 0; i <= N; i++)
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
                        printf("Thread not joined.\n");
                        exit(-1);
                    }
                }
                else if (i == 2)
                {
                    if (pthread_join(threads[i], &ret3) != 0)
                    {
                        printf("Thread not joined.\n");
                        exit(-1);
                    }
                }
                else if (i == 3)
                {
                    if (pthread_join(threads[i], &ret4) != 0)
                    {
                        printf("Thread not joined.\n");
                        exit(-1);
                    }
                }
                else if (i == 4)
                {
                    if (pthread_join(threads[i], &ret5) != 0)
                    {
                        printf("Thread not joined.\n");
                        exit(-1);
                    }
                }
                else if (i == 5)
                {
                    if (pthread_join(threads[i], &ret6) != 0)
                    {
                        printf("Thread not joined.\n");
                        exit(-1);
                    }
                }
                else if (i == 6)
                {
                    if (pthread_join(threads[i], &ret7) != 0)
                    {
                        printf("Thread not joined.\n");
                        exit(-1);
                    }
                }
            }

            float mode = *(float *)ret1;
            int sum = *(int *)ret2;
            int max = *(int *)ret3;
            float avg = *(float *)ret4;
            float median = *(float *)ret5;
            int secMax = *(int *)ret6;
            int tmin = *(int *)ret7;

            printf("\nMode: %f\n", mode);
            printf("Sum: %d\n", sum);
            printf("Max: %d\n", max);
            printf("Average: %f\n", avg);
            printf("Median: %f\n", median);
            printf("secMax: %d\n", secMax);
            printf("Third Min: %d\n", tmin);
        }
        else
        {
            printf("N is not greater than 0 and less than 7.\n");
        }
    }
    return 0;
}
