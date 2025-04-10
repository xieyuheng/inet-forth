#pragma once

struct principal_port_t {
    node_t *node;
    size_t index;
};

principal_port_t *principal_port_new(node_t *node, size_t index);
