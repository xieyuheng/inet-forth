#include "index.h"

object_spec_t wire_object_spec = {
    .name = "wire",
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
    if (*self_pointer == NULL) return;

    wire_t *self = *self_pointer;
    free(self);
    *self_pointer = NULL;
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

void wire_connect(wire_t *self, value_t value) {
    // TODO will be the critical section in multi-threaded implementation
    assert(!self->fuzed_value);
    self->fuzed_value = value;
}

value_t
follow_wire(value_t value) {
    while (is_wire(value)) {
        wire_t *wire = as_wire(value);
        value = wire->fuzed_value;
    }

    return value;
}

bool
is_fuzed(value_t x, value_t y) {
    return follow_wire(x) == follow_wire(y);
}
