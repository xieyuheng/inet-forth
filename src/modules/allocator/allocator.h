#pragma once

// Can be use to allocate value value in parallel,
// learned from section "6.4.3 Resource Allocator Caches"
// of the book "Is Parallel Programming Hard?".

// - allocate := pop  per-thread stack -- return value to caller
// - free     := push per-thread stack -- recycle value from caller

struct allocator_t {
    stack_t *main_stack;
};

allocator_t *allocator_new(void);
void allocator_destroy(allocator_t **self_pointer);
