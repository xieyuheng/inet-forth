#include "index.h"

struct node_allocator_t {
    allocator_t *allocator;
    size_t node_count;
    node_t *node_heap;
    array_t *per_thread_stack_array;
};

node_allocator_t *
node_allocator_new(size_t node_count) {
    node_allocator_t *self = new(node_allocator_t);

    self->allocator = allocator_new(NODE_ALLOCATOR_CACHE_SIZE);
    stack_t *node_stack = allocator_stack(self->allocator);

    self->node_count = node_count;
    self->node_heap = allocate(node_count * sizeof(node_t));
    for (size_t i = 0; i < node_count; i++) {
        node_t *node = &self->node_heap[i];
        node->id = i;
        node_init(node);
        stack_push(node_stack, node);
    }

    self->per_thread_stack_array = array_auto();
    return self;
}

void
node_allocator_destroy(node_allocator_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        node_allocator_t *self = *self_pointer;
        allocator_destroy(&self->allocator);
        free(self->node_heap);
        array_destroy(&self->per_thread_stack_array);
        free(self);
        *self_pointer = NULL;
    }
}

void
node_allocator_add_per_thread_stack(node_allocator_t *self, stack_t *stack) {
    array_push(self->per_thread_stack_array, stack);
}

size_t
node_allocator_thread_count(const node_allocator_t *self) {
    return array_length(self->per_thread_stack_array);
}
