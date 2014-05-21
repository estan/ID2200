#ifndef MALLOC_H
#define MALLOC_H

#include "malloc_impl.h"

#define malloc malloc_impl
#define realloc realloc_impl
#define free free_impl

#endif /* MALLOC_H */
