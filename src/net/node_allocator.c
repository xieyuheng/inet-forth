#include "index.h"

static void
prepare_one_batch_of_nodes(node_allocator_t *self) {
    mutex_lock(self->allocator->mutex);

    for (size_t i = 0; i < self->batch_size; i++) {
        node_t *node = node_new();
        node->id = ++self->node_count;
        stack_push(self->allocator->stack, node);
        array_push(self->node_array, node);
    }

    mutex_unlock(self->allocator->mutex);
}

node_allocator_t *
node_allocator_new(void) {
    size_t cache_size = NODE_ALLOCATOR_CACHE_SIZE;
    size_t batch_size = NODE_ALLOCATOR_BATCH_SIZE;

    node_allocator_t *self = new(node_allocator_t);
    self->allocator = allocator_new(cache_size);
    self->batch_size = batch_size;
    self->node_array = array_new_auto_with((destroy_fn_t *) node_destroy);
    prepare_one_batch_of_nodes(self);
    return self;
}

void
node_allocator_destroy(node_allocator_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    node_allocator_t *self = *self_pointer;
    allocator_destroy(&self->allocator);
    array_destroy(&self->node_array);
    free(self);
    *self_pointer = NULL;
}

node_t *
node_allocator_allocate(node_allocator_t *self, stack_t *stack) {
    node_t *node = allocator_maybe_allocate(self->allocator, stack);
    while (!node) {
        prepare_one_batch_of_nodes(self);
        node = allocator_maybe_allocate(self->allocator, stack);
    }

    node->is_allocated = true;
    return node;
}

void
node_allocator_recycle(node_allocator_t *self, stack_t *stack, node_t **node_pointer) {
    node_t *node = *node_pointer;
    node->is_allocated = false;
    array_purge(node->value_array);
    allocator_recycle(self->allocator, stack, (void **) node_pointer);
}
