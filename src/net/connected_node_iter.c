#include "index.h"

connected_node_iter_t *
connected_node_iter_new(node_allocator_t *node_allocator, node_t *root) {
    assert(root);
    connected_node_iter_t *self = new(connected_node_iter_t);
    self->node_allocator = node_allocator;
    self->root = root;
    self->adjacency_hash = node_adjacency_hash(node_allocator);
    self->occurred_node_set = set_new();
    self->remaining_node_list = list_new();
    return self;
}

void
connected_node_iter_destroy(connected_node_iter_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    connected_node_iter_t *self = *self_pointer;
    hash_destroy(&self->adjacency_hash);
    set_destroy(&self->occurred_node_set);
    list_destroy(&self->remaining_node_list);
    free(self);
    *self_pointer = NULL;
}

static void
take_node(connected_node_iter_t *self, node_t *node) {
    set_add(self->occurred_node_set, node);
    array_t *adjacency_array = hash_get(self->adjacency_hash, node);
    for (size_t i = 0; i < array_length(adjacency_array); i++) {
        list_push(self->remaining_node_list, array_get(adjacency_array, i));
    }
}

node_t *
connected_node_iter_first(connected_node_iter_t *self) {
    node_t *node = self->root;
    take_node(self, node);
    return node;
}

node_t *
connected_node_iter_next(connected_node_iter_t *self) {
    node_t *node = list_pop(self->remaining_node_list);
    if (!node) return NULL;

    if (set_has(self->occurred_node_set, node))
        return connected_node_iter_next(self);

    take_node(self, node);
    return node;
}

// array_t *
// connected_node_array(node_allocator_t *node_allocator) {

// }
