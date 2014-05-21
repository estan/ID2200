/*
 * Benchmark parameters:
 *
 *  USE_SYSTEM_MALLOC  Use system malloc/realloc/free instead of our own.
 *  STRATEGY           Allocation strategy (1 = First Fit, 2 = Best Fit).
 *  NUM_REPS           Number times to repeat the test.
 *  NUM_ALLOCS         Number of allocations in each repetition.
 *  MIN_SIZE           Minimum block size for each allocation.
 *  MAX_SIZE           Maximum block size for each allocation.
 */
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <time.h>

#include "malloc.h"
#include "brk.h"

void test_fragmentation() {
    int i;

    size_t size[NUM_ALLOCS];
    char *block[NUM_ALLOCS];

    size_t allocated = 0;
    void *heap_end_before, *heap_end_after;

    /* Generate block sizes */
    for (i = 0; i < NUM_ALLOCS; ++i) {
        int diff = MAX_SIZE - MIN_SIZE;
        size[i] = MIN_SIZE + (diff == 0 ? 0 : (rand() % diff));
    }

    heap_end_before = endHeap();

    /* Allocate blocks */
    for (i = 0; i < NUM_ALLOCS; ++i) {
        block[i] = malloc(size[i]);
        allocated += size[i];
    }

    heap_end_after = endHeap();

    /* Print allocated bytes and heap growth in bytes. */
    printf("%ld,%td\n", allocated, heap_end_after - heap_end_before);

    /* Free allocated blocks */
    for (i = 0; i < NUM_ALLOCS; ++i) {
        free(block[i]);
    }
}

int main(void) {
    int i;

    for (i = 0; i < NUM_REPS; ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            /* Run test in child process. */
            srand(time(NULL) ^ (getpid() << 16));
            test_fragmentation();
            exit(0);
        } else {
            /* Wait for child process to terminate */
            int status;
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}
