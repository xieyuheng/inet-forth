#include "index.h"

struct node_allocator_t {
    allocator_t *allocator;
    size_t node_count;
    size_t batch_size;
    array_t *node_array;
    array_t *per_thread_stack_array;
};

static void
prepare_one_batch_of_nodes(node_allocator_t *self) {
    mutex_lock(self->allocator->mutex);

    for (size_t i = 0; i < self->batch_size; i++) {
        node_t *node = new(node_t);
        node_init(node);
        node->id = self->node_count++;
        stack_push(self->allocator->stack, node);
    }

    mutex_unlock(self->allocator->mutex);
}

node_allocator_t *
node_allocator_new(size_t cache_size) {
    node_allocator_t *self = new(node_allocator_t);
    self->allocator = allocator_new(cache_size);
    self->batch_size = 1024;
    self->node_array = array_auto();
    // self->node_array = array_auto_with((destroy_fn_t *) node_destroy);
    prepare_one_batch_of_nodes(self);
    self->per_thread_stack_array = array_auto();
    return self;
}

void
node_allocator_destroy(node_allocator_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        node_allocator_t *self = *self_pointer;
        allocator_destroy(&self->allocator);
        array_destroy(&self->node_array);
        array_destroy(&self->per_thread_stack_array);
        free(self);
        *self_pointer = NULL;
    }
}

void
node_allocator_add_per_thread_stack(node_allocator_t *self, stack_t *stack) {
    array_push(self->per_thread_stack_array, stack);
}

size_t
node_allocator_thread_count(const node_allocator_t *self) {
    return array_length(self->per_thread_stack_array);
}

node_t *
node_allocator_allocate(node_allocator_t *self, stack_t *stack) {
    node_t *node = allocator_maybe_allocate(self->allocator, stack);
    while (!node) {
        prepare_one_batch_of_nodes(self);
        node = allocator_maybe_allocate(self->allocator, stack);
    }

    return node;
}

void
node_allocator_free(node_allocator_t *self, stack_t *stack, node_t *node) {
    allocator_free(self->allocator, stack, node);
}

void
node_allocator_recycle(node_allocator_t *self, stack_t *stack, node_t **node_pointer) {
    allocator_recycle(self->allocator, stack, (void **) node_pointer);
}
