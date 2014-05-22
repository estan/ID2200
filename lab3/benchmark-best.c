#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "timer.h"

#ifndef USE_SYSTEM_MALLOC
#include "malloc.h"
#endif

#define NUM_BLOCKS 5000

int main(int argc, char *argv[]) {
    int i;
    void *blocks[NUM_BLOCKS];
    srand(time(0));

    printf("op_nr,time\n");

    /* Measure allocation time */
    for (i = 0; i < NUM_BLOCKS; ++i) {
        usleep(rand() % 1000);
        TIMER_START();
        blocks[i] = malloc(16);
        TIMER_STOP();
        printf("%d,%ld\n", i, TIMER_ELAPSED_NS());
    }

    /* Free blocks */
    for (i = 0; i < NUM_BLOCKS; ++i) {
        free(blocks[i]);
    }

    return EXIT_SUCCESS;
}
