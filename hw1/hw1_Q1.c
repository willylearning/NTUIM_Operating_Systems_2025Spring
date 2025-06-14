#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void getFibonacci(int n) {
    if (n <= 0) return;
    
    long long fib[n]; // n >= 1
    fib[0] = 0;
    if (n > 1) fib[1] = 1;
    
    for (int i = 2; i < n; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
    
    for (int i = 0; i < n; i++) {
        if (i == 0) {
            printf("%lld", fib[i]);
        } else {
            printf(", %lld", fib[i]);
        } 
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <integer value>\n", argv[0]);
        return 1;
    }
    
    int n = atoi(argv[1]);
    if (n < 0) {
        fprintf(stderr,"Argument %d must be non-negative\n", n);
        return -1;
    }
    
    pid_t pid = fork(); // fork another process

    if (pid < 0) { // error occurred
        fprintf(stderr, "Fork Failed\n");
        return 1;
    } else if (pid == 0) { // child process
        getFibonacci(n);
        exit(0);
    } else { // parent process
        // parent will wait for the child to complete
        wait(NULL); 
        printf("Child Complete\n");
    }
    return 0;
}
