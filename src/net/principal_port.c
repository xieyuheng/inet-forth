#include "index.h"

principal_port_t *
principal_port_new(node_t *node, size_t index) {
    principal_port_t *self = new(principal_port_t);
    self->node = node;
    self->index = index;
    return self;
}

void
principal_port_destroy(principal_port_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    principal_port_t *self = *self_pointer;
    free(self);
    *self_pointer = NULL;
}
