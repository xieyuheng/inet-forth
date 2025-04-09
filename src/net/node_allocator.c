#include "index.h"

struct node_allocator_t {
    allocator_t *allocator;
    size_t next_node_id;
    size_t batch_size;
    size_t node_count;
    node_t *node_heap;
    array_t *per_thread_stack_array;
};

node_allocator_t *
node_allocator_new(size_t cache_size) {
    node_allocator_t *self = new(node_allocator_t);

    self->allocator = allocator_new(cache_size);    

    self->batch_size = 1024;

    self->node_count = NODE_COUNT;
    self->node_heap = allocate(self->node_count * sizeof(node_t));
    for (size_t i = 0; i < self->node_count; i++) {
        node_t *node = &self->node_heap[i];
        node->id = self->next_node_id;
        node_init(node);
        stack_push(self->allocator->stack, node);
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

node_t *
node_allocator_allocate(node_allocator_t *self, stack_t *stack) {
    return allocator_allocate(self->allocator, stack);
}

void
node_allocator_free(node_allocator_t *self, stack_t *stack, node_t *node) {
    allocator_free(self->allocator, stack, node);
}

void
node_allocator_recycle(node_allocator_t *self, stack_t *stack, node_t **node_pointer) {
    allocator_recycle(self->allocator, stack, (void **) node_pointer);
}
