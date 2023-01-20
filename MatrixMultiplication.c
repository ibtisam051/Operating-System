#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct Data
{
    int **mat1;
    int **mat2;
    int **result;

    int M;
    int N;
    int P;
};

void *MatrixMultiplication(void *ptr)
{
    struct Data Obj2 = *(struct Data *)ptr;

    for (int i = 0; i < Obj2.M; i++)
    {
        for (int j = 0; j < Obj2.P; j++)
        {
            Obj2.result[i][j] = 0;
        }
    }

    printf("\nBefore Resultant Matrix is:\n");
    for (int i = 0; i < Obj2.M; i++)
    {
        for (int j = 0; j < Obj2.P; j++)
        {
            printf("%d   ", Obj2.result[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < Obj2.M; i++)
    {
        for (int j = 0; j < Obj2.P; j++)
        {
            for (int k = 0; k < Obj2.N; k++)
            {
                Obj2.result[i][j] += Obj2.mat1[i][k] * Obj2.mat2[k][j];
            }
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
        Obj.M = atoi(argv[1]);
        Obj.N = atoi(argv[2]);
        Obj.P = atoi(argv[3]);

        printf("M: %d\n", Obj.M);
        printf("N: %d\n", Obj.N);
        printf("P: %d\n", Obj.P);

        //initializing Matrix 1
        Obj.mat1 = malloc(Obj.M * sizeof(*Obj.mat1));
        for (int i = 0; i < Obj.M; i++)
        {
            Obj.mat1[i] = malloc(Obj.N * sizeof(*Obj.mat1));
        }

        for (int i = 0; i < Obj.M; i++)
        {
            for (int j = 0; j < Obj.N; j++)
            {
                Obj.mat1[i][j] = rand() % 9;
            }
        }

        printf("\nMatrix 1 is:\n");
        for (int i = 0; i < Obj.M; i++)
        {
            for (int j = 0; j < Obj.N; j++)
            {
                printf("%d   ", Obj.mat1[i][j]);
            }
            printf("\n");
        }

        //initializing matrix 2
        Obj.mat2 = malloc(Obj.N * sizeof(*Obj.mat2));
        for (int i = 0; i < Obj.N; i++)
        {
            Obj.mat2[i] = malloc(Obj.P * sizeof(*Obj.mat2));
        }

        for (int i = 0; i < Obj.N; i++)
        {
            for (int j = 0; j < Obj.P; j++)
            {
                Obj.mat2[i][j] = rand() % 9;
            }
        }

        printf("\nMatrix 2 is:\n");
        for (int i = 0; i < Obj.N; i++)
        {
            for (int j = 0; j < Obj.P; j++)
            {
                printf("%d   ", Obj.mat1[i][j]);
            }
            printf("\n");
        }

        Obj.result = malloc(Obj.M * sizeof(*Obj.result));
        for (int i = 0; i < Obj.M; i++)
        {
            Obj.result[i] = malloc(Obj.P * sizeof(*Obj.result));
        }

        pthread_t threads[1];
        for (int i = 0; i < 1; i++)
        {
            if (pthread_create(&threads[i], NULL, &MatrixMultiplication, (void *)&Obj) != 0)
            {
                printf("Thread not created.\n");
                exit(-1);
            }
        }

        for (int i = 0; i < 1; i++)
        {
            if (pthread_join(threads[i], NULL) != 0)
            {
                printf("Thread not joined.\n");
                exit(-1);
            }
        }

        printf("\nFinal Resultant Matrix is:\n");
        for (int i = 0; i < Obj.M; i++)
        {
            for (int j = 0; j < Obj.P; j++)
            {
                printf("%d   ", Obj.result[i][j]);
            }
            printf("\n");
        }
    }
    return 0;
}