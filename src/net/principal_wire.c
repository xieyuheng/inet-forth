#include "index.h"

object_spec_t principal_wire_object_spec = {
    .name = "principal-wire",
    .print_fn = (print_fn_t *) principal_wire_print,
};

principal_wire_t *
principal_wire_new(node_t *node, size_t index) {
    principal_wire_t *self = new(principal_wire_t);
    self->spec = &principal_wire_object_spec;
    self->node = node;
    self->index = index;
    return self;
}

void
principal_wire_destroy(principal_wire_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    principal_wire_t *self = *self_pointer;
    free(self);
    *self_pointer = NULL;
}


bool
is_principal_wire(value_t value) {
    if (!is_xobject(value)) return false;
    return as_object(value)->spec == &principal_wire_object_spec;
}

principal_wire_t *
as_principal_wire(value_t value) {
    assert(is_principal_wire(value));
    return (principal_wire_t *) value;
}

void
principal_wire_print(const principal_wire_t *self, file_t *file) {
    assert(self->node);
    node_print(self->node, file);
    if (self->node->ctor) {
        fprintf(file, "-[%lu]!-<", self->index);
    } else {
        port_info_t *port_info = self->node->ctor->port_infos[self->index];
        fprintf(file, "-[%s]!-<", port_info->name);
    }
}
