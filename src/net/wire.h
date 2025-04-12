#pragma once

extern object_spec_t wire_object_spec;

struct wire_t {
    object_spec_t *spec;
    // might be `wire_t` or `principal_port_t`
    value_t fuzed_value;
};

wire_t *wire_new(void);
void wire_destroy(wire_t **self_pointer);

bool is_wire(value_t value);
wire_t *as_wire(value_t value);

value_t follow_wire(value_t value);
bool is_fuzed(value_t x, value_t y);
