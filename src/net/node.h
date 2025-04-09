#pragma once

struct node_t {
    const node_ctor_t *ctor;
    size_t id;
    bool is_allocated;
    array_t *value_array;
};

node_t *node_new(const node_ctor_t *ctor);
void node_destroy(node_t **self_pointer);

node_t *node_new_per_thread(node_allocator_t *node_allocator, stack_t *free_node_stack, const node_ctor_t *ctor);
void node_recycle_per_thread(node_allocator_t *node_allocator, stack_t *free_node_stack, node_t **self_pointer);

void node_set(node_t *self, size_t index, value_t value);
value_t node_get(const node_t *self, size_t index);

void node_print(const node_t *self, file_t *file);
