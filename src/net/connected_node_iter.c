#include "index.h"

connected_node_iter_t *
connected_node_iter_new(node_allocator_t *node_allocator, node_t *node) {
    connected_node_iter_t *self = new(connected_node_iter_t);
    self->node_allocator = node_allocator;
    self->node = node;
    self->adjacency_hash = node_adjacency_hash(node_allocator);
    return self;
}
