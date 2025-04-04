#include "index.h"

allocator_t *
allocator_new(void) {
    allocator_t *self = new_shared(allocator_t);
    self->main_stack = stack_new();
    return self;
}
