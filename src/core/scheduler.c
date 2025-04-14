#include "index.h"

scheduler_t *
scheduler_new(mod_t *mod, node_allocator_t *node_allocator, size_t worker_count) {
    scheduler_t *self = new(scheduler_t);
    self->mod = mod;
    self->node_allocator = node_allocator;
    self->worker_array = array_new_auto_with((destroy_fn_t *) worker_destroy);
    for (size_t i = 0; i < worker_count; i++) {
        worker_t *worker = worker_new(mod, node_allocator);
        worker->scheduler = self;
        worker->index = i;
        array_push(self->worker_array, worker);
    }

    self->worker_tid_array = array_new_auto();
    atomic_store(&self->atomic_task_count, 0);
    return self;
}

void
scheduler_destroy(scheduler_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    scheduler_t *self = *self_pointer;
    array_destroy(&self->worker_array);
    array_destroy(&self->worker_tid_array);
    free(self);
    *self_pointer = NULL;
}

size_t
scheduler_worker_count(scheduler_t *self) {
    return array_length(self->worker_array);
}
