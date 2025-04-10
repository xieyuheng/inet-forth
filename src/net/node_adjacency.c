#include "index.h"

node_adjacency_t *
node_adjacency_new(node_t *start_node, size_t start_port_index, size_t end_port_index, node_t *end_node) {
    node_adjacency_t *self = new(node_adjacency_t);
    self->start_node = start_node;
    self->start_port_index = start_port_index;
    self->end_port_index = end_port_index;
    self->end_node = end_node;
    return self;
}

void
node_adjacency_destroy(node_adjacency_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    node_adjacency_t *self = *self_pointer;
    free(self);
    *self_pointer = NULL;
}

hash_t *
build_node_adjacency_hash(node_allocator_t *node_allocator) {
    hash_t *adjacency_hash = hash_new();
    hash_set_destroy_fn(adjacency_hash, (destroy_fn_t *) array_destroy);

    array_t *node_array = allocated_node_array(node_allocator);
    size_t length = array_length(node_array);
    for (size_t i = 0; i < length; i++) {
        node_t *x = array_get(node_array, i);
        array_t *adjacency_array = array_auto();
        hash_set(adjacency_hash, x, adjacency_array);

        for (size_t j = 0; j < length; j++) {
            node_t *y = array_get(node_array, j);
            if (node_is_adjacent(x, y)) {
                array_push(adjacency_array, y);
            }
        }
    }

    array_destroy(&node_array);
    return adjacency_hash;
}
