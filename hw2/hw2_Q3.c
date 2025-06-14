#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int unsorted[] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8};
int length = sizeof(unsorted) / sizeof(unsorted[0]);
int *sorted;

// interval saves [start, end] for lists in sorting threads
typedef struct {
    int start;
    int end;
} interval;

// Bubble Sort
void bubble_sort(int *a, int start, int end) {
    for (int i = start; i <= end; i++) {
        for (int j = start; j < end - (i - start); j++) {
            if (a[j] > a[j + 1]) { 
                // swap a[j] and a[j + 1]
                int tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
            }
        }
    }
}

void *sortingThread(void *param) {
    interval *i = (interval *)param;
    bubble_sort(unsorted, i->start, i->end);
    pthread_exit(0);
}

void *mergingThread(void *param) {
    int i = 0, j = (length - 1) / 2 + 1, k = 0;

    while (i <= (length - 1) / 2 && j <= length - 1) {
        if (unsorted[i] <= unsorted[j]) {
            sorted[k] = unsorted[i];
            i++;
        } else {
            sorted[k] = unsorted[j];
            j++;
        }
        k++;
    }

    while (i <= (length - 1) / 2) {
        sorted[k++] = unsorted[i++];
    }
    while (j <= length - 1) {
        sorted[k++] = unsorted[j++];
    }
    pthread_exit(0);
}

int main() {
    sorted = (int*)malloc(length * sizeof(int));

    pthread_t sorting1_tid, sorting2_tid, merge_tid;

    // sublist 1
    interval *i1 = (interval *)malloc(sizeof(interval));
    i1->start = 0;
    i1->end = (length - 1) / 2;

    // sublist 2
    interval *i2 = (interval *)malloc(sizeof(interval));
    i2->start = (length - 1) / 2 + 1;
    i2->end = length - 1;

    // create the sorting threads
    pthread_create(&sorting1_tid, NULL, sortingThread, i1);
    pthread_create(&sorting2_tid, NULL, sortingThread, i2);
    // wait for the sorting threads to exit
    pthread_join(sorting1_tid, NULL);
    pthread_join(sorting2_tid, NULL);

    // create the merging thread
    pthread_create(&merge_tid, NULL, mergingThread, NULL);
    // wait for the merging thread to exit
    pthread_join(merge_tid, NULL);

    printf("Sorted list: ");
    for (int i = 0; i < length; i++) {
        printf("%d ", sorted[i]);
    }
    printf("\n");

    free(i1);
    free(i2);
    free(sorted);

    return 0;
}

