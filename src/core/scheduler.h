#pragma once

struct scheduler_t {
    mod_t *mod;
    size_t worker_pool_size;
    worker_t **workers;
    size_t task_queue_size;
    queue_t **task_queues;
    // scheduler thread
    thread_id_t thread_id;
    // worker threads
    worker_ctx_t **worker_ctxs;

};

scheduler_t *scheduler_new(mod_t *mod, size_t worker_pool_size);
void scheduler_destroy(scheduler_t **self_pointer);
