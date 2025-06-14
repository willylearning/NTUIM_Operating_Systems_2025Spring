#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int circlePoints = 0; // number of points inside the circle

void *generatePoints(void *param) {
    int n = *(int *)param; // total number of points
    for (int i = 0; i < n; i++) {
        // Generate a random point (x, y) in the range [-1, 1]
        double x = (double)rand() / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand() / RAND_MAX * 2.0 - 1.0;
        if (x * x + y * y <= 1) {
            circlePoints++;
        }
    }
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    pthread_t tid; // the thread identifier 
    pthread_attr_t attr; // set of attributes for the thread

    if (argc != 2) {
        fprintf(stderr,"usage: a.out <integer value>\n");
        return -1;
    }

    int totalPoints = atoi(argv[1]); // total number of points
    if (totalPoints <= 0) {
        fprintf(stderr,"Argument %d must be positive\n", totalPoints);
        return -1;
    }

    pthread_attr_init(&attr); //  get the default attributes
    pthread_create(&tid, &attr, generatePoints, &totalPoints); // create the thread
    pthread_join(tid, NULL); // wait for the thread to exit

    double pi = (double)(4 * circlePoints) / totalPoints;
    printf("Approximate value of pi = %f\n", pi);

    return 0;
}