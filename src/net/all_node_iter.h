#pragma once

struct all_node_iter_t {
    node_allocator_t *node_allocator;
    size_t cursor;
};

all_node_iter_t *all_node_iter_new(node_allocator_t *node_allocator);
void all_node_iter_destroy(all_node_iter_t **self_pointer);

node_t *all_node_iter_first(all_node_iter_t *self);
node_t *all_node_iter_next(all_node_iter_t *self);
