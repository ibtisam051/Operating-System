#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

long int noOfProcesses = 0;
long int size = 10000;
int extraArray[10000];
int x = 0;
int newSize = 0;

int *DivideArray(int *arr)
{
    for (int j = 0; j < size / noOfProcesses; j++) // 5 times
    {
        arr[j] = arr[x];
        x++;
    }
    return arr;
}

int *DivideArray2(int *arr, int s)
{
    for (int j = 0; j < s; j++) // 5 times
    {
        arr[j] = arr[x];
        x++;
    }
    return arr;
}

int main(int argc, char **argv)
{
    if (argc - 1 == 0)
    {
        printf("No Input.\n");
    }
    else
    {
        noOfProcesses = atoi(argv[1]);
        printf("No of Processes are: %ld\n", noOfProcesses);

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

        if (size % noOfProcesses == 0)
        {                                           // for array update counter
            int y = 0;                              // for extraArray update counter
            for (int i = 0; i < noOfProcesses; i++) // 4 no. of processes
            {
                // FOR FIRST PROCESS
                int fd1[2];
                int fd2[2];

                pipe(fd1);
                pipe(fd2);

                int *arr = DivideArray(ptr);

                pid_t pid = fork();
                if (pid == 0) // child process
                {
                    close(fd1[1]);
                    close(fd2[0]);

                    int *child = (int *)malloc((size / noOfProcesses) * sizeof(int));
                    read(fd1[0], child, (4 * (size / noOfProcesses)));

                    int temp = 0;
                    for (int j = 0; j < size / noOfProcesses; j++)
                    {
                        for (int k = j + 1; k < size / noOfProcesses; k++)
                        {
                            if (child[j] > child[k])
                            {
                                temp = child[j];
                                child[j] = child[k];
                                child[k] = temp;
                            }
                        }
                    }

                    printf("\nArray sorted for child %d: ", i + 1);
                    for (int l = 0; l < size / noOfProcesses; l++)
                    {
                        printf("%d ", child[l]);
                    }

                    write(fd2[1], child, (4 * (size / noOfProcesses)));
                    exit(1);
                }
                else if (pid > 0) // parent process
                {
                    close(fd1[0]);
                    close(fd2[1]);

                    write(fd1[1], arr, (4 * (size / noOfProcesses))); // 4*5 = 20

                    int *temp = (int *)malloc((size / noOfProcesses) * sizeof(int));
                    read(fd2[0], temp, (4 * (size / noOfProcesses)));

                    for (int j = 0; j < size / noOfProcesses; j++)
                    {
                        extraArray[y] = temp[j];
                        y++;
                    }

                    wait(NULL);
                }
                else
                {
                    printf("Error.\n");
                    exit(-1);
                }
            }

            printf("\n\nBefore Final Array: ");
            for (int k = 0; k < size; k++)
            {
                printf("%d ", extraArray[k]);
            }
            int blue = 0;
            for (int j = 0; j < size; j++)
            {
                for (int k = j + 1; k < size; k++)
                {
                    if (extraArray[j] > extraArray[k])
                    {
                        blue = extraArray[j];
                        extraArray[j] = extraArray[k];
                        extraArray[k] = blue;
                    }
                }
            }

            printf("\n\nAfter Final Array: ");
            for (int k = 0; k < size; k++)
            {
                printf("%d ", extraArray[k]);
            }
            printf("\n");
        }
        else
        {
            printf("Odd case.\n");

            int y = 0;                              // for extraArray update counter
            for (int i = 0; i < noOfProcesses; i++) // 4 no. of processes
            {
                // FOR FIRST PROCESS
                int fd1[2];
                int fd2[2];

                pipe(fd1);
                pipe(fd2);

                if (i != noOfProcesses - 1)
                {
                    int *arr = DivideArray(ptr);

                    pid_t pid = fork();
                    if (pid == 0) // child process
                    {
                        close(fd1[1]);
                        close(fd2[0]);

                        int *child = (int *)malloc((size / noOfProcesses) * sizeof(int));
                        read(fd1[0], child, (4 * (size / noOfProcesses)));

                        int temp = 0;
                        for (int j = 0; j < size / noOfProcesses; j++)
                        {
                            for (int k = j + 1; k < size / noOfProcesses; k++)
                            {
                                if (child[j] > child[k])
                                {
                                    temp = child[j];
                                    child[j] = child[k];
                                    child[k] = temp;
                                }
                            }
                        }

                        printf("\nArray sorted for child %d: ", i);
                        for (int l = 0; l < size / noOfProcesses; l++)
                        {
                            printf("%d ", child[l]);
                        }

                        write(fd2[1], child, (4 * (size / noOfProcesses)));
                        exit(1);
                    }
                    else if (pid > 0) // parent process
                    {
                        close(fd1[0]);
                        close(fd2[1]);

                        write(fd1[1], arr, (4 * (size / noOfProcesses))); // 4*5 = 20

                        int *temp = (int *)malloc((size / noOfProcesses) * sizeof(int));
                        read(fd2[0], temp, (4 * (size / noOfProcesses)));

                        for (int j = 0; j < size / noOfProcesses; j++)
                        {
                            extraArray[y] = temp[j];
                            y++;
                        }

                        wait(NULL);
                    }
                    else
                    {
                        printf("Error.\n");
                        exit(-1);
                    }
                }
                else if (i == noOfProcesses - 1)
                {
                    newSize = size - y;

                    int *newarr = DivideArray2(ptr, newSize);

                    pid_t pid = fork();
                    if (pid == 0) // child process
                    {
                        close(fd1[1]);
                        close(fd2[0]);

                        int *child = (int *)malloc((newSize) * sizeof(int));
                        read(fd1[0], child, (4 * (newSize)));

                        int temp = 0;
                        for (int j = 0; j < newSize; j++)
                        {
                            for (int k = j + 1; k < newSize; k++)
                            {
                                if (child[j] > child[k])
                                {
                                    temp = child[j];
                                    child[j] = child[k];
                                    child[k] = temp;
                                }
                            }
                        }

                        printf("\nArray sorted for child %d: ", i);
                        for (int l = 0; l < newSize; l++)
                        {
                            printf("%d ", child[l]);
                        }

                        write(fd2[1], child, (4 * (newSize)));
                        exit(1);
                    }
                    else if (pid > 0) // parent process
                    {
                        close(fd1[0]);
                        close(fd2[1]);

                        write(fd1[1], newarr, (4 * (newSize))); // 4*5 = 20

                        int *temp = (int *)malloc((newSize) * sizeof(int));
                        read(fd2[0], temp, (4 * (newSize)));

                        for (int j = 0; j < newSize; j++)
                        {
                            extraArray[y] = temp[j];
                            y++;
                        }

                        wait(NULL);
                    }
                    else
                    {
                        printf("Error.\n");
                        exit(-1);
                    }
                }
            }

            printf("\n\nBefore Final Array: ");
            for (int k = 0; k < size; k++)
            {
                printf("%d ", extraArray[k]);
            }
            int blue = 0;
            for (int j = 0; j < size; j++)
            {
                for (int k = j + 1; k < size; k++)
                {
                    if (extraArray[j] > extraArray[k])
                    {
                        blue = extraArray[j];
                        extraArray[j] = extraArray[k];
                        extraArray[k] = blue;
                    }
                }
            }

            printf("\n\nAfter Final Array: ");
            for (int k = 0; k < size; k++)
            {
                printf("%d ", extraArray[k]);
            }
            printf("\n");
        }
    }
    return 0;
}