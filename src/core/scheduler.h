#pragma once

struct scheduler_t {
    mod_t *mod;
    node_allocator_t *node_allocator;
    array_t *worker_array;
};

scheduler_t *scheduler_new(mod_t *mod, node_allocator_t *node_allocator, size_t worker_count);
