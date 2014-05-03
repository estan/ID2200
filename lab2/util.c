/**
 * Utility functions definitions.
 *
 * See util.h for documentation.
 *
 * @author Elvis Stansvik <stansvik@kth.se>
 * @author Sam Henriksson <hensa@kth.se>
 */
#include <string.h>
#include <ctype.h>
#include <sys/time.h>

int is_empty(const char *str) {
    if (!str) {
        return 1;
    }
    while (*str) {
        if (!isspace(*str)) {
            return 0;
        }
        ++str;
    }
    return 1;
}

int match(const char *s1, const char *s2) {
    int s1_len = strlen(s1);
    return s1_len == strlen(s2) && strncmp(s1, s2, s1_len) == 0;
}

double elapsed_ms(const struct timeval *t0, const struct timeval *t1) {
    return (t1->tv_sec * 1000 + (double)t1->tv_usec / 1000) -
           (t0->tv_sec * 1000 + (double)t0->tv_usec / 1000);
}

