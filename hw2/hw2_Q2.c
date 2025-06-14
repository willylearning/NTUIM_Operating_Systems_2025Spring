#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int *fib; // array for placing the Fibonacci sequence that can be shared by threads

void *getFibonacci(void *param) {
    int n = *(int *)param; // total number of Fibonacci numbers to generate
    fib[0] = 0;
    if (n > 1) fib[1] = 1;
    
    for (int i = 2; i < n; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    pthread_t tid; // the thread identifier 
    pthread_attr_t attr; // set of attributes for the thread

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number of Fibonacci numbers>\n", argv[0]);
        return 1;
    }
    
    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr,"Argument %d must be positive\n", n);
        return -1;
    }

    fib = (int *)malloc(n * sizeof(int)); // allocate memory for the Fibonacci sequence

    pthread_attr_init(&attr); // get the default attributes
    pthread_create(&tid, &attr, getFibonacci, &n); // create the thread
    pthread_join(tid, NULL); // wait for the thread to exit

    printf("Fibonacci sequence: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", fib[i]);
    }
    printf("\n");

    free(fib); // free the allocated memory
    return 0;
}
