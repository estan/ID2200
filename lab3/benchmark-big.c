#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "timer.h"
#include "big_blocks.h"

#ifndef USE_SYSTEM_MALLOC
#include "malloc.h"
#endif

int main(int argc, char *argv[]) {
    int i;
    void *blocks[NUM_BIG_BLOCKS];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <prefix>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Measure allocation of big blocks */
    for (i = 0; i < NUM_BIG_BLOCKS; ++i) {
        TIMER_START();
        blocks[i] = malloc(big_block_sizes[i]);
        TIMER_STOP();
        printf("%s-big,%ld\n", argv[1], TIMER_ELAPSED_NS());
    }

    /* Free blocks */
    for (i = 0; i < NUM_BIG_BLOCKS; ++i) {
        free(blocks[i]);
    }

    return EXIT_SUCCESS;
}
