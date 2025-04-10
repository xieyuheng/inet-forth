#pragma once

struct connected_node_iter_t {
    node_t *root;
    hash_t *node_adjacency_hash;
    set_t *occurred_node_set;
    list_t *remaining_node_list;
};

connected_node_iter_t *connected_node_iter_new(node_t *root, hash_t *node_adjacency_hash);
void connected_node_iter_destroy(connected_node_iter_t **self_pointer);

node_t *connected_node_iter_first(connected_node_iter_t *self);
node_t *connected_node_iter_next(connected_node_iter_t *self);
