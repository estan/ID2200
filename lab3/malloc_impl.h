#ifndef MALLOC_IMPL_H
#define MALLOC_IMPL_H

void *malloc_impl(size_t);
void free_impl(void *);
void *realloc_impl(void *, size_t);

#endif /* MALLOC_IMPL_H */
