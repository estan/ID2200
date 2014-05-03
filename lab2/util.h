/**
 * Utility functions declarations.
 *
 * @author Elvis Stansvik <stansvik@kth.se>
 * @author Sam Henriksson <hensa@kth.se>
 */
#ifndef UTIL_H
#define UTIL_H

#include <sys/time.h>

/**
 * Returns 1 if @a str is empty or just whitespace, otherwise 0.
 */
int is_empty(const char *str);

/**
 * Returns 1 if @a s1 and @a s2 match each other, otherwise 0.
 */
int match(const char *s1, const char *s2);

/**
 * Returns the number of milliseconds between @a t0 and @a t1.
 */
double elapsed_ms(const struct timeval *t0, const struct timeval *t1);

#endif // UTIL_H
