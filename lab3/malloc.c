#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

#define MIN_UNITS 1024

/* The most restrictive type of the machine. */
typedef long align_t;

/* Block header data structure. */
typedef union header_u {
    struct {
        union header_u *next; /* Next free block list. */
        size_t size;          /* Size of block, including header. */
    } s;
    align_t dummy; /* Force alignment to align_t boundaries. */
} header_t;

static header_t base = { { &base, 0 } }; /* Base of free list. */
static header_t *freep = &base;          /* Free block pointer. */
static void *heap_end = 0;               /* End of heap. */

/* Returns a pointer to the end of the heap. */
void *endHeap() {
    if (heap_end == 0)
        heap_end = sbrk(0);
    return heap_end;
}

/*
 * Adds a new block of least min(nunits, MIN_UNITS) * sizeof(header_t) bytes
 * of memory to the free list. Returns a pointer to the added block, or NULL
 * if the request failed.
 */
static header_t *add_memory(size_t nunits) {
    const void *mem;
    header_t *header;
    size_t npages;

    const static int flags = MAP_SHARED | MAP_ANONYMOUS;
    const static int protection = PROT_READ | PROT_WRITE;
    const long page_size = sysconf(_SC_PAGESIZE);

    if (nunits < MIN_UNITS)
        nunits = MIN_UNITS;

    if (heap_end == 0)
        heap_end = sbrk(0);

    npages = (nunits * sizeof(header_t) - 1) / page_size + 1;
    mem = mmap(heap_end, npages * page_size, protection, flags, -1, 0);
    if (mem == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }
    nunits = (npages * page_size) / sizeof(header_t);
    heap_end += npages * page_size;
    header = (header_t *)mem;
    header->s.size = nunits;
    free(header + 1);
    return freep;
}

/* Allocates size bytes and return a pointer to the allocated memory. */
void *malloc(size_t size) {
    header_t *p, *prevp;

    /* Calculate number of header-sized units required. */
    size_t nunits = (size + sizeof(header_t) - 1) / sizeof(header_t) + 1;

    if (size == 0)
        return NULL; /* No-op */

    for (prevp = freep, p = prevp->s.next; ; prevp = p, p = p->s.next) {
        if (p->s.size >= nunits) {
            /* p is big enough. */
            if (p->s.size == nunits) {
                /* p is exactly big enough, remove from free list. */
                prevp->s.next = p->s.next;
            } else {
                /* p is more than big enough, allocate from tail end. */
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp;
            return (void *)(p + 1);
        }
        if (p == freep) {
            /* No suitably large block found. */
            if ((p = add_memory(nunits)) == NULL) {
                /* Out of memory */
                return NULL;
            }
        }
    }
}

/* Frees the memory pointed to by ptr. */
void free(void *ptr) {
    header_t *freed, *p;

    if (ptr == NULL) {
        return; /* No-op */
    }

    freed = (header_t *) ptr - 1; /* Point to block header. */

    /* Find insertion point p for bp. */
    for (p = freep; !(p < freed && freed < p->s.next); p = p->s.next) {
        if (p >= p->s.next && (p < freed || freed < p->s.next)) {
            /* Insertion point is either start or end of list. */
            break;
        }
    }
    if (freed + freed->s.size == p->s.next) {
        /* Merge with upper neighbor. */
        freed->s.size += p->s.next->s.size;
        freed->s.next = p->s.next->s.next;
    } else {
        freed->s.next = p->s.next;
    }
    if(p + p->s.size == freed) {
        /* Merge with lower neighbor. */
        p->s.size += freed->s.size;
        p->s.next = freed->s.next;
    } else {
        p->s.next = freed;
    }
    freep = p;
}

/* Change size of memory block pointer to by ptr to size bytes. */
void *realloc(void *ptr, size_t size) {
    header_t *old_header;
    size_t old_size;
    void *new_ptr;

    if (ptr == NULL) {
        return malloc(size);
    }
    if (size == 0) {
        free(ptr);
        return NULL;
    }

    /* Get old header and size. */
    old_header = (header_t *) ptr - 1;
    old_size = (old_header->s.size - 1) * sizeof(header_t);

    if (size == old_size) {
        return ptr; /* No-op */
    }

    /* Allocate new memory, copy contents and free old memory. */
    if ((new_ptr = malloc(size)) == NULL) {
        return NULL;
    }
    memcpy(new_ptr, ptr, size < old_size ? size : old_size);
    free(ptr);

    return new_ptr;
}
