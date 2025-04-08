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
    size_t expected_allocation_count = 64 * 1024;
    self->allocator = allocator_new(expected_allocation_count);
    return self;
}
