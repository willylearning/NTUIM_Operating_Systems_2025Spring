#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void getCollatz(int n) {
    while (n != 1) {
        printf("%d, ", n);
        if (n % 2 == 0) { // n is even
            n /= 2;  
        } else { // n is odd
            n = 3*n + 1; 
        }
    }
    printf("1\n"); 
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive integer>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n < 0) {
        fprintf(stderr,"Argument %d must be non-negative\n", n);
        return -1;
    }

    pid_t pid = fork(); // fork another process
    
    if (pid < 0) {  // error occurred
        fprintf(stderr, "Fork Failed\n");
        return 1;
    } else if (pid == 0) {  // child process
        getCollatz(n);
        exit(0);
    } else {  // parent process
        // parent will wait for the child to complete
        wait(NULL);
        printf("Child Complete\n");
    }

    return 0;
}
