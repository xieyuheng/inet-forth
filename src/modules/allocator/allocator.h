#pragma once

// Can be use to allocate value value in parallel,
// learned from section "6.4.3 Resource Allocator Caches"
// of the book "Is Parallel Programming Hard?".

// - allocate := pop  per-thread stack -- return value to caller
// - free     := push per-thread stack -- recycle value from caller

allocator_t *allocator_new(size_t expected_allocation_count);
void allocator_destroy(allocator_t **self_pointer);

void *allocator_stack(allocator_t *self);

void *allocator_maybe_allocate(allocator_t *self, stack_t *stack);
void *allocator_allocate(allocator_t *self, stack_t *stack);

void allocator_free(allocator_t *self, stack_t *stack, void *value);
void allocator_recycle(allocator_t *self, stack_t *stack, void **value_pointer);
