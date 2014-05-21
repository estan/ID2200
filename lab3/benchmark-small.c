#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "timer.h"
#include "small_blocks.h"

#ifndef USE_SYSTEM_MALLOC
#include "malloc.h"
#endif

int main(int argc, char *argv[]) {
    int i;
    void *blocks[NUM_SMALL_BLOCKS];
    long total_time = 0;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <prefix>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Measure allocation of small blocks */
    for (i = 0; i < NUM_SMALL_BLOCKS; ++i) {
        TIMER_START();
        blocks[i] = malloc(small_block_sizes[i]);
        TIMER_STOP();
        printf("%s-small,%ld\n", argv[1], TIMER_ELAPSED_NS());
        total_time += TIMER_ELAPSED_NS();
    }

    /* Free blocks */
    for (i = 0; i < NUM_SMALL_BLOCKS; ++i) {
        free(blocks[i]);
    }

    return EXIT_SUCCESS;
}
