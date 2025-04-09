#pragma once

struct allocated_node_iter_t {
    node_allocator_t *node_allocator;
    size_t cursor;
};

allocated_node_iter_t *allocated_node_iter_new(node_allocator_t *node_allocator);
void allocated_node_iter_destroy(allocated_node_iter_t **self_pointer);

node_t *allocated_node_iter_first(allocated_node_iter_t *self);
node_t *allocated_node_iter_next(allocated_node_iter_t *self);

array_t *all_node_array(node_allocator_t *node_allocator);
