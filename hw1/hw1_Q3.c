#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void getCollatz(int *shm_ptr, int n) {
    int idx = 0;
    while (n != 1) {
        shm_ptr[idx++] = n;
        if (n % 2 == 0) { // n is even
            n /= 2;  
        } else { // n is odd
            n = 3*n + 1; 
        }
    }
    shm_ptr[idx] = 1;  
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

    const int SIZE = 4096;
    const char *shm_name = "/collatz"; // name of the shared memory object

    // a. Establish the shared-memory object
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);

    ftruncate(shm_fd, SIZE);

    int *shm_ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0); // pointer to shared memory object

    // b. Create the child process and wait for it to terminate
    pid_t pid = fork(); // fork another process

    if (pid < 0) {  // error occurred
        fprintf(stderr, "Fork Failed\n");
        return 1;
    } else if (pid == 0) {  // child process
        getCollatz(shm_ptr, n);
        exit(0);
    } else {  // parent process
        // parent will wait for the child to complete
        wait(NULL); 

        // c. Ouput the content of shared memory
        for (int i = 0; shm_ptr[i] != 1; i++) {
            printf("%d, ", shm_ptr[i]);
        }
        printf("1\n"); // print the last number 1
        
        // d. Remove the shared-memory object
        munmap(shm_ptr, SIZE);
        close(shm_fd);
        shm_unlink(shm_name);

        printf("Child Complete\n");
    }

    return 0;
}
