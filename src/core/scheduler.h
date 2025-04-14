#pragma once

struct scheduler_t {
    mod_t *mod;
    node_allocator_t *node_allocator;
    array_t *worker_array;
};
