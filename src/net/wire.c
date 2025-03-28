#include "index.h"

object_spec_t wire_object_spec = {
    .name = "wire",
    .print_fn = (print_fn_t *) wire_print,
};

wire_t *
wire_new(void) {
    wire_t *self = new(wire_t);
    self->spec = &wire_object_spec;
    return self;
}

void
wire_destroy(wire_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        wire_t *self = *self_pointer;
        free(self);
        // Does NOT own `node` and `opposite`.
        *self_pointer = NULL;
    }
}

bool
is_wire(value_t value) {
    if (!is_xobject(value)) return false;
    return as_object(value)->spec == &wire_object_spec;
}

wire_t *
as_wire(value_t value) {
    assert(is_wire(value));
    return (wire_t *) value;
}

wire_t *
wire_opposite(const wire_t *self) {
    return atomic_load(&self->atomic_opposite);
}

void
wire_set_opposite(wire_t *self, wire_t *opposite) {
    atomic_store(&self->atomic_opposite, opposite);
}

const char *
wire_name(const wire_t *self) {
    assert(self->node);
    port_info_t *port_info = self->node->ctor->port_infos[self->index];
    assert(port_info);
    return port_info->name;
}

const char *
wire_node_name(const wire_t *self) {
    assert(self->node);
    return self->node->ctor->name;
}

void
wire_free_from_node(wire_t *self) {
    self->node = NULL;
    atomic_store(&self->atomic_is_principal, false);
}

bool
wire_is_free(const wire_t *self) {
    if (self->node) return false;
    return true;
}

bool
wire_is_principal(const wire_t *self) {
    return atomic_load(&self->atomic_is_principal);
}

void
wire_print_left(const wire_t *self, file_t *file) {
    if (!self->node) {
        fprintf(file, "-<");
        return;
    }

    node_print(self->node, file);

    if (wire_is_principal(self)) {
        fprintf(file, "-%s!-<", wire_name(self));
    } else {
        fprintf(file, "-%s-<", wire_name(self));
    }
}

void
wire_print_right(const wire_t *self, file_t *file) {
    if (!self->node) {
        fprintf(file, ">-");
        return;
    }

    if (wire_is_principal(self)) {
        fprintf(file, ">-!%s-", wire_name(self));
    } else {
        fprintf(file, ">-%s-", wire_name(self));
    }

    node_print(self->node, file);
}

void
wire_print(const wire_t *self, file_t *file) {    
    if (wire_opposite(self))
        wire_print_left(wire_opposite(self), file);
    wire_print_right(self, file);
}

void
wire_print_reverse(const wire_t *self, file_t *file) {
    wire_print_left(self, file);
    if (wire_opposite(self))
        wire_print_right(wire_opposite(self), file);
}

void
wire_print_net(wire_t *self, file_t *file) {
    fprintf(file, "<net>\n");

    wire_iter_t *iter = wire_iter_new(self);
    wire_t *wire = wire_iter_first(iter);

    fprintf(file, "<root>\n");
    wire_print(wire, file);
    fprintf(file, "\n");
    fprintf(file, "</root>\n");

    wire = wire_iter_next(iter);

    fprintf(file, "<body>\n");
    while (wire) {
        wire_print(wire, file);
        fprintf(file, "\n");
        wire = wire_iter_next(iter);
    }

    wire_iter_destroy(&iter);
    fprintf(file, "</body>\n");

    fprintf(file, "</net>\n");
}
