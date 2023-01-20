#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct Data
{
    int *ptr;
    int size;
};

void *Even(void *ptr)
{
    struct Data Obj2 = *(struct Data *)ptr;

    int *sum = (int *)malloc(sizeof(int));

    //printf("\nEven numbers are: ");
    for (int i = 0; i < Obj2.size; i++)
    {
        if (Obj2.ptr[i] % 2 == 0)
        {
            //printf("%d ", Obj2.ptr[i]);
            *sum += Obj2.ptr[i];
        }
    }

    return (void *)sum;
}

void *Odd(void *ptr)
{
    struct Data Obj2 = *(struct Data *)ptr;

    //int *sum = (int *)malloc(sizeof(int));
    printf("\nOdd numbers are: ");
    for (int i = 0; i < Obj2.size; i++)
    {
        if (Obj2.ptr[i] % 2 != 0)
        {
            printf("%d ", Obj2.ptr[i]);
        }
    }
    return 0;
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
        Obj.ptr = (int *)malloc((argc - 1) * sizeof(int));

        Obj.size = argc - 1;

        for (int i = 0; i < argc - 1; i++)
        {
            Obj.ptr[i] = atoi(argv[i + 1]);
        }

        printf("Array is: ");
        for (int i = 0; i < argc - 1; i++)
        {
            printf("%d ", Obj.ptr[i]);
        }

        pthread_t thd1, thd2;

        void *retVal1;
        if (pthread_create(&thd1, NULL, &Even, (void *)&Obj) != 0)
        {
            printf("Thread not created.\n");
            exit(-1);
        }
        if (pthread_create(&thd2, NULL, &Odd, (void *)&Obj) != 0)
        {
            printf("Thread not created.\n");
            exit(-1);
        }

        if (pthread_join(thd1, &retVal1) != 0)
        {
            printf("Thread not joined.\n");
            exit(-1);
        }

        if (pthread_join(thd2, NULL) != 0)
        {
            printf("Thread not joined.\n");
            exit(-1);
        }

        int sum = *(int *)retVal1;
        printf("Sum is: %d ", sum);
    }
    return 0;
}