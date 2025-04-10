#pragma once

struct node_adjacency_t {
    node_t *start_node;
    size_t start_port_index;
    size_t end_port_index;
    node_t *end_node;
};

node_adjacency_t *node_adjacency_new(node_t *start_node, size_t start_port_index, size_t end_port_index, node_t *end_node);
void node_adjacency_destroy(node_adjacency_t **self_pointer);

hash_t *build_node_adjacency_hash(node_allocator_t *node_allocator);
