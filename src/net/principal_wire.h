#pragma once

extern object_spec_t principal_wire_object_spec;

struct principal_wire_t {
    object_spec_t *spec;
    node_t *node;
    size_t index;
    principal_wire_t *oppsite;
};

principal_wire_t *principal_wire_new(node_t *node, size_t index);
void principal_wire_destroy(principal_wire_t **self_pointer);

bool is_principal_wire(value_t value);
principal_wire_t *as_principal_wire(value_t value);

void principal_wire_print_left(const principal_wire_t *self, file_t *file);
void principal_wire_print_right(const principal_wire_t *self, file_t *file);
