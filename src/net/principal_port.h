#pragma once

extern object_spec_t principal_port_object_spec;

struct principal_port_t {
    object_spec_t *spec;
    node_t *node;
    size_t index;
};

principal_port_t *principal_port_new(node_t *node, size_t index);
void principal_port_destroy(principal_port_t **self_pointer);

bool is_principal_port(value_t value);
principal_port_t *as_principal_port(value_t value);
