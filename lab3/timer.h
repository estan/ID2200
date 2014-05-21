#ifndef TIMER_H
#define TIMER_H

struct timespec t0, t1; /* Start/end time for timer */

/* Timer macros */
#define TIMER_START() clock_gettime(CLOCK_MONOTONIC_RAW, &t0)
#define TIMER_STOP() clock_gettime(CLOCK_MONOTONIC_RAW, &t1)
#define TIMER_ELAPSED_NS() \
    (t1.tv_sec * 1000000000 + t1.tv_nsec) - \
    (t0.tv_sec * 1000000000 + t0.tv_nsec)

#endif /* TIMER_H */
