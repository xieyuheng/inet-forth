#pragma once

// Can be use to allocate value value in parallel,
// learned from section "6.4.3 Resource Allocator Caches"
// of the book "Is Parallel Programming Hard?".

// - allocate := pop  per-thread stack -- return value to caller
// - free     := push per-thread stack -- recycle value from caller

struct allocator_t {
    mutex_t *mutex;
    stack_t *stack;
    size_t cache_size;
};

allocator_t *allocator_new(size_t cache_size);
void allocator_destroy(allocator_t **self_pointer);

void *allocator_maybe_allocate(allocator_t *self, stack_t *stack);
void *allocator_allocate(allocator_t *self, stack_t *stack);

void allocator_recycle(allocator_t *self, stack_t *stack, void **value_pointer);
