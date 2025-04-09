#pragma once

struct node_iter_t {
    node_allocator_t *node_allocator;
    size_t cursor;
};

node_iter_t *node_iter_new(node_allocator_t *node_allocator);
void node_iter_destroy(node_iter_t **self_pointer);

node_t *node_iter_first(node_iter_t *self);
node_t *node_iter_next(node_iter_t *self);
