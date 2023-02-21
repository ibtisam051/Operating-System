#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct thread_args {
    int start;
    int end;
    int *factorial;
};

void *calculate_factorial(void *args) {
    struct thread_args *t_args = (struct thread_args *) args;
    int i;
    int result = 1;
    for (i = t_args->start; i <= t_args->end; i++) {
        result *= i;
    }
    *(t_args->factorial) *= result;
    printf("Thread %ld calculated %d-%d: %d\n", pthread_self(), t_args->start, t_args->end, result);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s N M\n", argv[0]);
        exit(1);
    }
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    if (n < m) {
        printf("Error: N must be greater than or equal to M\n");
        exit(1);
    }
    pthread_t threads[m];
    int factorial = 1;
    int i, range, start = 1;
    for (i = 0; i < m; i++) {
        range = (n - start + 1) / (m - i);
        struct thread_args *args = (struct thread_args *) malloc(sizeof(struct thread_args));
        args->start = start;
        args->end = start + range - 1;
        args->factorial = &factorial;
        pthread_create(&threads[i], NULL, calculate_factorial, (void *) args);
        start += range;
    }
    for (i = 0; i < m; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Factorial of %d is %d\n", n, factorial);
    return 0;
}
