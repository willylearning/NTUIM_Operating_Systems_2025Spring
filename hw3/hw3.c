#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int circlePoints = 0;
pthread_mutex_t lock; 

void *generatePoints(void *param) {
    unsigned int seed = time(NULL) ^ pthread_self(); // use thread ID to seed random number generator

    for (int i = 0; i < 1000; i++) { // every thread generates 1000 random points
        double x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;

        if (x * x + y * y <= 1.0) {
            pthread_mutex_lock(&lock);
            circlePoints++;
            pthread_mutex_unlock(&lock);
        }
    }

    pthread_exit(0);
}

int main() {
    pthread_t threads[5];
    pthread_mutex_init(&lock, NULL);

    // create 5 threads
    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, generatePoints, NULL);
    }

    // wait for all threads to finish
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    // destroy the mutex
    pthread_mutex_destroy(&lock);

    int totalPoints = 5 * 1000; // 5 threads * 1000 points each
    double pi = (double)(4 * circlePoints) / totalPoints;

    printf("Approximate value of pi = %f\n", pi);
    return 0;
}
