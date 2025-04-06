#include "index.h"

struct allocator_t {
    mutex_t *mutex;
    stack_t *value_stack;
    size_t expected_allocation_count;
};

allocator_t *
allocator_new(size_t expected_allocation_count) {
    allocator_t *self = new_page_aligned(allocator_t);
    self->mutex = mutex_new();
    self->value_stack = stack_new();
    self->expected_allocation_count = expected_allocation_count;
    return self;
}

void
allocator_destroy(allocator_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        allocator_t *self = *self_pointer;
        mutex_destroy(&self->mutex);
        stack_destroy(&self->value_stack);
        free(self);
        *self_pointer = NULL;
    }
}

void *
allocator_allocate(allocator_t *self, stack_t *value_stack) {
    if (stack_is_empty(value_stack)) {
        mutex_lock(self->mutex);

        size_t count = 0;
        while (count < self->expected_allocation_count) {
            if (stack_is_empty(self->value_stack)) break;

            stack_push(value_stack, stack_pop(self->value_stack));
            count++;
        }

        mutex_unlock(self->mutex);
    }

    if (stack_is_empty(value_stack)) {
        fprintf(stderr, "[allocator_allocate] not enough value\n");
        exit(1);
    }

    return stack_pop(value_stack);
}

void
allocator_free(allocator_t *self, stack_t *value_stack, void *value) {
    if (stack_length(value_stack) >= 2 * self->expected_allocation_count) {
        mutex_lock(self->mutex);

        size_t count = 0;
        while (count < self->expected_allocation_count) {
            stack_push(self->value_stack, stack_pop(value_stack));
            count++;
        }

        mutex_unlock(self->mutex);
    }

    stack_push(value_stack, value);
}
