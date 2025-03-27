#pragma once

// A worker is like a little forth inner interpreter.

struct worker_t {
    mod_t *mod;
    list_t *token_list;
    queue_t *task_queue;
    stack_t *value_stack;
    stack_t *return_stack;
    size_t node_id_count;
    // wire and node should use managed memory,
    // because some nets have no root.
    // i simply record the pointers in worker for now.
    set_t *node_set;
};

worker_t *worker_new(mod_t *mod);
void worker_destroy(worker_t **self_pointer);

void worker_print_return_stack(const worker_t *self, file_t *file);
void worker_print_value_stack(const worker_t *self, file_t *file);
void worker_print(const worker_t *self, file_t *file);

void worker_connect_top_wire_pair(worker_t *self);
void worker_maybe_schedule_task(worker_t *self, wire_t *first_wire, wire_t *second_wire);

node_t *worker_add_node(worker_t* self, const node_ctor_t *ctor);
void worker_delete_node(worker_t* self, node_t *node);

wire_t *worker_add_wire(worker_t* self);
void worker_delete_wire(worker_t* self, wire_t *wire);

wire_t *worker_wire_connect(worker_t* self, wire_t *first_wire, wire_t *second_wire);
