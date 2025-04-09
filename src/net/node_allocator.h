#pragma once

node_allocator_t *node_allocator_new(void);
void node_allocator_destroy(node_allocator_t **self_pointer);

node_t *node_allocator_allocate(node_allocator_t *self, stack_t *stack);
void node_allocator_free(node_allocator_t *self, stack_t *stack, node_t *node);
void node_allocator_recycle(node_allocator_t *self, stack_t *stack, node_t **node_pointer);
