#include "index.h"

scheduler_t *
scheduler_new(mod_t *mod, size_t worker_pool_size) {
    scheduler_t *self = new_page_aligned(scheduler_t);
    self->mod = mod;

    self->node_allocator = node_allocator_new();

    self->worker_pool_size = worker_pool_size;
    self->workers = allocate_pointers(worker_pool_size);
    self->worker_ctxs = allocate_pointers(worker_pool_size);
    for (size_t i = 0; i < worker_pool_size; i++) {
        self->workers[i] = worker_new(mod, self->node_allocator);
        self->workers[i]->scheduler = self;
        self->workers[i]->index = i;
        self->worker_ctxs[i] = worker_ctx_new(self->workers[i]);
    }

    self->task_queue_size = SCHEDULER_TASK_QUEUE_SIZE;
    self->task_queues = allocate_pointers(worker_pool_size);
    for (size_t i = 0; i < worker_pool_size; i++) {
        self->task_queues[i] = queue_new_with(
            self->task_queue_size,
            (destroy_fn_t *) task_destroy);
    }

    self->garbage_wire_arrays = allocate_pointers(worker_pool_size);
    for (size_t i = 0; i < worker_pool_size; i++) {
        self->garbage_wire_arrays[i] =
            array_auto_with((destroy_fn_t *) wire_destroy);
    }

    return self;
}

void
scheduler_destroy(scheduler_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    scheduler_t *self = *self_pointer;

    for (size_t i = 0; i < self->worker_pool_size; i++)
        worker_destroy(&self->workers[i]);
    free(self->workers);

    for (size_t i = 0; i < self->worker_pool_size; i++)
        worker_ctx_destroy(&self->worker_ctxs[i]);
    free(self->worker_ctxs);

    for (size_t i = 0; i < self->worker_pool_size; i++)
        queue_destroy(&self->task_queues[i]);
    free(self->task_queues);

    for (size_t i = 0; i < self->worker_pool_size; i++)
        array_destroy(&self->garbage_wire_arrays[i]);
    free(self->garbage_wire_arrays);

    free(self);
    *self_pointer = NULL;
}
