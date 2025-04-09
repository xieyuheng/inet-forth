#pragma once

struct node_allocator_t {
    allocator_t *allocator;
    size_t node_count;
    size_t batch_size;
    array_t *node_array;
};

node_allocator_t *node_allocator_new(void);
void node_allocator_destroy(node_allocator_t **self_pointer);

node_t *node_allocator_allocate(node_allocator_t *self, stack_t *stack);
void node_allocator_recycle(node_allocator_t *self, stack_t *stack, node_t **node_pointer);
