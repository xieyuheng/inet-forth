#pragma once

struct worker_ctx_t {
    worker_t *worker;
    tid_t tid;
};

worker_ctx_t *worker_ctx_new(worker_t *worker);
void worker_ctx_destroy(worker_ctx_t **self_pointer);
