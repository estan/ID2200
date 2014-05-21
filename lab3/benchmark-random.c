#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "timer.h"
#include "random_blocks.h"

#ifndef USE_SYSTEM_MALLOC
#include "malloc.h"
#endif

int main(int argc, char *argv[]) {
    int i;
    void *blocks[NUM_RANDOM_BLOCKS];

    /* Measure allocation time */
    for (i = 0; i < NUM_RANDOM_BLOCKS; ++i) {
        TIMER_START();
        blocks[i] = malloc(random_block_sizes[i]);
        TIMER_STOP();
        printf("%d,%ld\n", i, TIMER_ELAPSED_NS());
    }

    /* Free blocks */
    for (i = 0; i < NUM_RANDOM_BLOCKS; ++i) {
        free(blocks[i]);
    }

    return EXIT_SUCCESS;
}
