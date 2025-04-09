#pragma once

struct connected_node_iter_t {
    node_t *node;
    node_allocator_t *node_allocator;
    hash_t *adjacency_hash;
};
