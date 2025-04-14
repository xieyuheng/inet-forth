#pragma once

struct scheduler_t {
    mod_t *mod;
    node_allocator_t *node_allocator;
    array_t *worker_array;
    array_t *worker_tid_array;
};

scheduler_t *scheduler_new(mod_t *mod, node_allocator_t *node_allocator, size_t worker_count);
void scheduler_destroy(scheduler_t **self_pointer);
