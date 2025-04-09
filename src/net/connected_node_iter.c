#include "index.h"

connected_node_iter_t *
connected_node_iter_new(node_allocator_t *node_allocator, node_t *node) {
    connected_node_iter_t *self = new(connected_node_iter_t);
    self->node_allocator = node_allocator;
    self->node = node;
    self->adjacency_hash = node_adjacency_hash(node_allocator);
    return self;
}

void
connected_node_iter_destroy(connected_node_iter_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    connected_node_iter_t *self = *self_pointer;
    hash_destroy(&self->adjacency_hash);
    free(self);
    *self_pointer = NULL;
}

// node_t *
// connected_node_iter_first(connected_node_iter_t *self) {

// }

// node_t *
// connected_node_iter_next(connected_node_iter_t *self) {

// }

// array_t *
// connected_node_array(node_allocator_t *node_allocator) {
// }
