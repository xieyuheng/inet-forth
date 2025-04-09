#pragma once

struct connected_node_iter_t {
    node_allocator_t *node_allocator;
    node_t *node;
    hash_t *adjacency_hash;
};

connected_node_iter_t *connected_node_iter_new(node_allocator_t *node_allocator, node_t *node);
void connected_node_iter_destroy(connected_node_iter_t **self_pointer);

node_t *connected_node_iter_first(connected_node_iter_t *self);
node_t *connected_node_iter_next(connected_node_iter_t *self);

array_t *connected_node_array(node_allocator_t *node_allocator);
