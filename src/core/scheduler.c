#include "index.h"

scheduler_t *
scheduler_new(mod_t *mod, node_allocator_t *node_allocator, size_t worker_count) {
    scheduler_t *self = new(scheduler_t);
    self->mod = mod;
    self->node_allocator = node_allocator;
    self->worker_array = array_new_auto_with((destroy_fn_t *) worker_destroy);
    for (size_t count = 0; count < worker_count; count++) {
        array_push(self->worker_array, worker_new(mod, node_allocator));
    }

    return self;
}

void
scheduler_destroy(scheduler_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    scheduler_t *self = *self_pointer;
    array_destroy(&self->worker_array);
    free(self);
    *self_pointer = NULL;
}
