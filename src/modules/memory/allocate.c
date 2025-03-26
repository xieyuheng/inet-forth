#include "index.h"

// The address of a block returned by malloc or realloc in GNU systems
// is always a multiple of eight (or sixteen on 64-bit systems).
// -- https://www.gnu.org/software/libc/manual/html_node/Aligned-Memory-Blocks.html

bool
pointer_is_8_bytes_aligned(void *pointer) {
    return (((uintptr_t) pointer) & ((uintptr_t) 0x7)) == 0;
}

void *
allocate(size_t size) {
    void *pointer = calloc(1, size);
    assert(pointer);
    assert(pointer_is_8_bytes_aligned(pointer));
    return pointer;
}

void *
allocate_many(size_t size, size_t unit_size) {
    void *pointer = calloc(size, unit_size);
    assert(pointer);
    assert(pointer_is_8_bytes_aligned(pointer));
    return pointer;
}

void *
allocate_pointers(size_t size) {
    return allocate_many(size, sizeof(void *));
}

void *
reallocate(void *pointer, size_t old_size, size_t new_size) {
    void *new_pointer = realloc(pointer, new_size);
    assert(new_pointer);
    assert(pointer_is_8_bytes_aligned(new_pointer));
    memset((((char *) new_pointer) + old_size), 0, new_size - old_size);
    return new_pointer;
}

void *
reallocate_many(void *pointer, size_t old_size, size_t new_size, size_t unit_size) {
    void *new_pointer = realloc(pointer, new_size * unit_size);
    assert(new_pointer);
    assert(pointer_is_8_bytes_aligned(new_pointer));
    memset((((char *) new_pointer) + old_size * unit_size),
           0,
           (new_size - old_size) * unit_size);
    return new_pointer;
}

void *
reallocate_pointers(void *pointer, size_t old_size, size_t new_size) {
    return reallocate_many(pointer, old_size, new_size, sizeof(void *));
}

bool
pointer_is_cache_line_aligned(void *pointer) {
    size_t cache_line_size = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
    return (((uintptr_t) pointer) % cache_line_size) == 0;
}

void *
allocate_shared(size_t size) {
    size_t cache_line_size = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
    assert(cache_line_size > 0);
    size_t real_size = ((size / cache_line_size) + 1) * cache_line_size;
    void *pointer = aligned_alloc(cache_line_size, real_size);
    memset(pointer, 0, real_size);
    assert(pointer);
    assert(pointer_is_8_bytes_aligned(pointer));
    assert(pointer_is_cache_line_aligned(pointer));
    return pointer;
}
