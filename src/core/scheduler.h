#pragma once

struct scheduler_t {
    mod_t *mod;
    node_allocator_t *node_allocator;
    array_t *worker_array;
    array_t *worker_tid_array;
    atomic_size_t atomic_task_count;
};

scheduler_t *scheduler_new(mod_t *mod, node_allocator_t *node_allocator, size_t worker_count);
void scheduler_destroy(scheduler_t **self_pointer);

size_t scheduler_worker_count(scheduler_t *self);

void scheduler_start(scheduler_t *scheduler, thread_fn_t *worker_thread_fn);
void scheduler_wait(scheduler_t *scheduler);
