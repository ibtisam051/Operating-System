#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
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

    // sorting in ascending order
    int temp = 0;
    for (int i = 0; i < 10000; i++)
    {
        for (int j = i + 1; j < 10000; j++)
        {
            if (ptr[i] > ptr[j])
            {
                temp = ptr[i];
                ptr[i] = ptr[j];
                ptr[j] = temp;
            }
        }
    }

    printf("\n\nSorted Array is: ");
    for (int i = 0; i < 10000; i++)
    {
        printf("%d ", ptr[i]);
    }
    printf("\n");
    return 0;
}