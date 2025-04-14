#include "index.h"

object_spec_t wire_object_spec = {
    .name = "wire",
};

wire_t *
wire_new(void) {
    wire_t *self = new(wire_t);
    self->spec = &wire_object_spec;
    atomic_store(&self->atomic_fuzed_value, NULL);
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

static value_t
walk(value_t value) {
    while (is_wire(value)) {
        wire_t *wire = as_wire(value);
        value_t fuzed_value = atomic_load(&wire->atomic_fuzed_value);
        if (!fuzed_value) break;
        
        value = fuzed_value;
    }

    return value;
}

bool
is_fuzed(value_t x, value_t y) {
    x = walk(x);
    y = walk(y);
    return x == y;
}

bool
is_connected(value_t x, value_t y) {
    x = walk(x);
    y = walk(y);

    if (is_principal_wire(x) && is_principal_wire(y)) {
        return (x == y ||
                (as_principal_wire(x)->oppsite == y &&
                 as_principal_wire(y)->oppsite == x));
    } else {
        return x == y;
    }
}
