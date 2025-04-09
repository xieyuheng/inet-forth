#include "index.h"

node_iter_t *
node_iter_new(node_allocator_t *node_allocator) {
    node_iter_t *self = new(node_iter_t);
    self->node_allocator = node_allocator;
    self->cursor = 0;
    return self;
}

void
node_iter_destroy(node_iter_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    node_iter_t *self = *self_pointer;
    free(self);
    *self_pointer = NULL;
}
