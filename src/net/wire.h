#pragma once

extern object_spec_t wire_object_spec;

struct wire_t {
    object_spec_t *spec;
    // might be `wire_t` or `principal_wire_t`
    atomic_value_t atomic_fuzed_value;
};

wire_t *wire_new(void);
void wire_destroy(wire_t **self_pointer);

bool is_wire(value_t value);
wire_t *as_wire(value_t value);

bool is_fuzed(value_t x, value_t y);
bool is_connected(value_t x, value_t y);
