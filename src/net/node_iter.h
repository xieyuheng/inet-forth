#pragma once

struct node_iter_t {
    node_allocator_t *node_allocator;
    size_t cursor;
};

node_iter_t *node_iter_new(node_allocator_t *node_allocator);
