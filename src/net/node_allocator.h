#pragma once

node_allocator_t *node_allocator_new(size_t cache_size);
void node_allocator_destroy(node_allocator_t **self_pointer);

void node_allocator_add_per_thread_stack(node_allocator_t *self, stack_t *stack);
size_t node_allocator_thread_count(const node_allocator_t *self);

node_t *node_allocator_allocate(node_allocator_t *self, stack_t *stack);
void node_allocator_free(node_allocator_t *self, stack_t *stack, node_t *node);
void node_allocator_recycle(node_allocator_t *self, stack_t *stack, node_t **node_pointer);
