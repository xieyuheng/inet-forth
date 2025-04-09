#pragma once

struct node_iter_t {
    node_allocator_t *node_allocator;
    size_t cursor;
};
