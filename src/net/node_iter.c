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

node_t *
node_iter_first(node_iter_t *self) {
    if (array_is_empty(self->node_allocator->node_array))
        return NULL;

    self->cursor = 0;
    node_t *node = array_get(self->node_allocator->node_array, self->cursor++);
    while (node && !node->is_allocated) {
        node = array_get(self->node_allocator->node_array, self->cursor++);
    }

    return node;
}

node_t *
node_iter_next(node_iter_t *self) {
    if (self->cursor >= array_length(self->node_allocator->node_array))
        return NULL;

    node_t *node = array_get(self->node_allocator->node_array, self->cursor++);
    while (node && !node->is_allocated) {
        node = array_get(self->node_allocator->node_array, self->cursor++);
    }

    return node;
}
