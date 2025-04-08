#include "index.h"

struct node_allocator_t {
    allocator_t *allocator;
    size_t size;
    node_t *node_heap;
};

node_allocator_t *
node_allocator_new(size_t size) {
    node_allocator_t *self = new(node_allocator_t);
    self->size = size;
    size_t cache_size = 64 * 1024;
    self->allocator = allocator_new(cache_size);
    return self;
}

void
node_allocator_destroy(node_allocator_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        node_allocator_t *self = *self_pointer;
        allocator_destroy(&self->allocator);
        free(self->node_heap);
        free(self);
        *self_pointer = NULL;
    }
}
