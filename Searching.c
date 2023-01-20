#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/file.h>
#include <string.h>

struct Data
{
    char *filename;
    int N;
    char *string;
};

void *Search(void *ptr)
{
    struct Data Obj2 = *(struct Data *)ptr;

    char word[100];
    int flag = 0;

    FILE *file = fopen(Obj2.filename, "r");

    if (file != NULL)
    {
        while (1)
        {
            fscanf(file, "%s", word);

            if (strcmp(word, Obj2.string) == 0)
            {
                printf("\nWord Found in the file.\n");
                flag = 0;
            }
            else
            {
                flag = 1;
            }

            if (feof(file))
            {
                break;
            }
        }
    }
    else
    {
        printf("File not Open.\n");
    }
    fclose(file);

    if (flag == 1)
    {
        printf("\nWord Not Found in the file.\n");
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
        Obj.filename = argv[1];
        Obj.N = atoi(argv[2]);
        Obj.string = argv[3];

        printf("FileName: %s ", Obj.filename);
        printf("\nN: %d ", Obj.N);
        printf("\nString: %s ", Obj.string);

        pthread_t threads[Obj.N];
        for (int i = 0; i < Obj.N; i++)
        {
            if (pthread_create(&threads[i], NULL, &Search, (void *)&Obj) != 0)
            {
                printf("Thread not created.\n");
                exit(-1);
            }
        }

        for (int i = 0; i < Obj.N; i++)
        {
            if (pthread_join(threads[i], NULL) != 0)
            {
                printf("Thread not created.\n");
                exit(-1);
            }
        }
    }
    return 0;
}