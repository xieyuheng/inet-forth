#pragma once

extern object_spec_t wire_object_spec;

struct wire_t {
    object_spec_t *spec;
    node_t *node;
    size_t index;
    wire_t *opposite;
};

wire_t *wire_new(void);
void wire_destroy(wire_t **self_pointer);

bool is_wire(value_t value);
wire_t *as_wire(value_t value);

wire_t *wire_opposite(const wire_t *self);
void wire_set_opposite(wire_t *self, wire_t *opposite);

bool wire_is_principal(const wire_t *self);

void wire_print(const wire_t *self, file_t *file);
