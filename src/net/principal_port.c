#include "index.h"

principal_port_t *
principal_port_new(node_t *node, size_t index) {
    principal_port_t *self = new(principal_port_t);
    self->node = node;
    self->index = index;
    return self;
}
