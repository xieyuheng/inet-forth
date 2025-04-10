#include "index.h"

object_spec_t principal_port_object_spec = {
    .name = "principal-port",
    // .print_fn = (print_fn_t *) principal_port_print,
};

principal_port_t *
principal_port_new(node_t *node, size_t index) {
    principal_port_t *self = new(principal_port_t);
    self->spec = &principal_port_object_spec;
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
