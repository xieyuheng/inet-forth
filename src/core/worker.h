#pragma once

// A worker is like a little forth inner interpreter.

struct worker_t {
    mod_t *mod;
    list_t *token_list;
    queue_t *task_queue;
    mutex_t *task_queue_front_mutex;
    stack_t *value_stack;
    stack_t *return_stack;
    node_allocator_t *node_allocator;
    stack_t *free_node_stack;
    scheduler_t *scheduler;
    size_t index;
    size_t victim_cursor;
};

worker_t *worker_new(mod_t *mod, node_allocator_t *node_allocator);
void worker_destroy(worker_t **self_pointer);

node_t *worker_new_node(worker_t* self, const node_ctor_t *ctor);
void worker_recycle_node(worker_t* self, node_t *node);

task_t *connect(value_t left, value_t right);
void worker_add_task(worker_t *worker, task_t *task);
void worker_reconnect_node(worker_t *worker, node_t *node);
void worker_disconnect_node(worker_t *worker, node_t *node);

node_t *worker_lookup_node_by_wire(worker_t* self, wire_t *wire);

void worker_print_return_stack(const worker_t *self, file_t *file);
void worker_print_value_stack(const worker_t *self, file_t *file);
void worker_print(const worker_t *self, file_t *file);
