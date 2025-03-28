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
    // to free wires at the end of each batch,
    // to avoid data race:
    // - read (during is principal query)
    // - free (during connecting two wires)
    array_t **garbage_wire_arrays;
};

scheduler_t *scheduler_new(mod_t *mod, size_t worker_pool_size);
void scheduler_destroy(scheduler_t **self_pointer);
