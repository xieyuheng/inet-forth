#include "index.h"

allocator_t *
allocator_new(void) {
    allocator_t *self = new_shared(allocator_t);
    self->main_stack = stack_new();
    return self;
}

void
allocator_destroy(allocator_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        allocator_t *self = *self_pointer;
        stack_destroy(&self->main_stack);
        free(self);
        *self_pointer = NULL;
    }
}
