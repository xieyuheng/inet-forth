#include "index.h"

struct allocator_t {
    mutex_t *mutex;
    stack_t *main_stack;
    size_t expected_size;
    array_t *per_thread_stack_array;
};

allocator_t *
allocator_new(size_t expected_size) {
    allocator_t *self = new_page_aligned(allocator_t);
    self->mutex = mutex_new();
    self->main_stack = stack_new();
    self->expected_size = expected_size;
    self->per_thread_stack_array = array_auto();
    return self;
}

void
allocator_destroy(allocator_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        allocator_t *self = *self_pointer;
        mutex_destroy(&self->mutex);
        stack_destroy(&self->main_stack);
        array_destroy(&self->per_thread_stack_array);
        free(self);
        *self_pointer = NULL;
    }
}
