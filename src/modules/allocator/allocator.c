#include "index.h"

struct allocator_t {
    mutex_t *mutex;
    stack_t *main_stack;
};

allocator_t *
allocator_new(void) {
    allocator_t *self = new_shared(allocator_t);
    self->mutex = mutex_new();
    self->main_stack = stack_new();
    return self;
}

void
allocator_destroy(allocator_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        allocator_t *self = *self_pointer;
        mutex_destroy(&self->mutex);
        stack_destroy(&self->main_stack);
        free(self);
        *self_pointer = NULL;
    }
}
