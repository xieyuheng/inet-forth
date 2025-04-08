#include "index.h"

static list_t *
lex_code(const char *code) {
    lexer_t *lexer = lexer_new();
    lexer->enable_int = true;
    lexer->enable_float = true;
    lexer->enable_string = true;
    lexer->line_comment = "//";
    lexer->string = code;
    lexer_run(lexer);

    list_t *token_list = lexer->token_list;
    lexer_destroy(&lexer);
    return token_list;
}

worker_t *
worker_new(mod_t *mod) {
    worker_t *self = new(worker_t);
    self->mod = mod;
    self->token_list = lex_code(mod->code);
    self->task_queue = queue_new_with(WORKER_TASK_QUEUE_SIZE, (destroy_fn_t *) task_destroy);
    // TODO We should use value_destroy to create value_stack.
    self->value_stack = stack_new();
    self->return_stack = stack_new_with((destroy_fn_t *) frame_destroy);
    self->debug_node_set = set_new();
    self->node_id_count = 0;

    return self;
}

void
worker_destroy(worker_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        worker_t *self = *self_pointer;
        list_destroy(&self->token_list);
        queue_destroy(&self->task_queue);
        stack_destroy(&self->value_stack);
        stack_destroy(&self->return_stack);
        set_destroy(&self->debug_node_set);
        free(self);
        *self_pointer = NULL;
    }
}

void
worker_print(const worker_t *self, file_t *file) {
    fprintf(file, "<worker>\n");

    size_t task_queue_length = queue_length(self->task_queue);
    fprintf(file, "<task-queue length=\"%lu\">\n", task_queue_length);
    for (size_t i = 0; i < task_queue_length; i++) {
        task_t *task = queue_get(self->task_queue, i);
        wire_print(task->wire, file);
        fprintf(file, "\n");
    }
    fprintf(file, "</task-queue>\n");

    worker_print_return_stack(self, file);
    worker_print_value_stack(self, file);

    fprintf(file, "</worker>\n");
}

void
worker_print_return_stack(const worker_t *self, file_t *file) {
    size_t return_stack_length = stack_length(self->return_stack);
    fprintf(file, "<return-stack length=\"%lu\">\n", return_stack_length);
    for (size_t i = 0; i < return_stack_length; i++) {
        frame_t *frame = stack_get(self->return_stack, i);
        frame_print(frame, file);
    }

    fprintf(file, "</return-stack>\n");
}

void
worker_print_value_stack(const worker_t *self, file_t *file) {
    size_t value_stack_length = stack_length(self->value_stack);
    fprintf(file, "<value-stack length=\"%lu\">\n", value_stack_length);
    for (size_t i = 0; i < value_stack_length; i++) {
        value_t value = stack_get(self->value_stack, i);
        value_print(value, file);
        fprintf(file, "\n");
    }

    fprintf(file, "</value-stack>\n");
}

void
worker_connect_top_wire_pair(worker_t *self) {
    wire_t *second_wire = stack_pop(self->value_stack);
    wire_t *first_wire = stack_pop(self->value_stack);

    wire_t *first_opposite = worker_wire_connect(self, second_wire, first_wire);

    if (wire_is_principal(first_opposite)) {
        maybe_return_task(self, first_opposite);
    }
}

void
maybe_return_task(worker_t *self, wire_t *wire) {
    wire_t *opposite = wire_opposite(wire);
    if (wire_is_principal(wire) && wire_is_principal(opposite)) {
        assert(wire_opposite(wire) == opposite);
        assert(wire_opposite(opposite) == wire);

        const rule_t *rule = mod_find_rule(self->mod, wire, opposite);
        if (!rule) return;

        worker_return_task(self, task_new(wire, rule));
    }
}

node_t *
worker_add_node(worker_t* self, const node_ctor_t *ctor) {
    node_t *node = node_new(ctor, ++self->node_id_count);

    if (core_debug_flag)
        set_add(self->debug_node_set, node);

    return node;
}

void
worker_delete_node(worker_t* self, node_t *node) {
    if (core_debug_flag)
        set_delete(self->debug_node_set, node);

    node_destroy(&node);
}

wire_t *
worker_add_wire(worker_t* self) {
    (void) self;
    wire_t *wire = wire_new();
    return wire;
}

void
worker_delete_wire(worker_t* self, wire_t *wire) {
    if (worker_is_in_pool(self)) {
        array_t *garbage_wire_array =
            self->scheduler->garbage_wire_arrays[self->index];
        array_push(garbage_wire_array, wire);
    } else {
        wire_destroy(&wire);
    }
}

wire_t *
worker_wire_connect(worker_t* self, wire_t *first_wire, wire_t *second_wire) {
    wire_t *first_opposite = wire_opposite(first_wire);
    wire_t *second_opposite = wire_opposite(second_wire);

    wire_set_opposite(first_opposite, second_opposite);
    wire_set_opposite(second_opposite, first_opposite);

    worker_delete_wire(self, first_wire);
    worker_delete_wire(self, second_wire);

    return first_opposite;
}

bool
worker_is_in_pool(const worker_t* self) {
    return self->scheduler != NULL;
}

void
worker_return_task(worker_t* self, task_t *task) {
    if (worker_is_in_pool(self)) {
        queue_enqueue(self->scheduler->task_queues[self->index], task);
    } else {
        queue_enqueue(self->task_queue, task);
    }
}
