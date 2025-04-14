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
worker_new(mod_t *mod, node_allocator_t *node_allocator) {
    worker_t *self = new(worker_t);
    self->mod = mod;
    self->token_list = lex_code(mod->code);
    self->task_queue = queue_new_with(WORKER_TASK_QUEUE_SIZE, (destroy_fn_t *) task_destroy);
    // TODO We should use value_destroy to create value_stack.
    self->value_stack = stack_new();
    self->return_stack = stack_new_with((destroy_fn_t *) frame_destroy);

    self->node_allocator = node_allocator;
    self->free_node_stack = stack_new();

    return self;
}

void
worker_destroy(worker_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    worker_t *self = *self_pointer;
    list_destroy(&self->token_list);
    queue_destroy(&self->task_queue);
    stack_destroy(&self->value_stack);
    stack_destroy(&self->return_stack);
    free(self);
    *self_pointer = NULL;
}

void
worker_print(const worker_t *self, file_t *file) {
    fprintf(file, "<worker>\n");

    size_t task_queue_length = queue_length(self->task_queue);
    fprintf(file, "<task-queue length=\"%lu\">\n", task_queue_length);
    for (size_t i = 0; i < task_queue_length; i++) {
        task_t *task = queue_get(self->task_queue, i);
        (void) task;
        // TODO
        // task_print(task, file);
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
worker_connect(worker_t *self, value_t left, value_t right) {
    if (is_principal_wire(left) && is_principal_wire(right)) {
        as_principal_wire(left)->oppsite = as_principal_wire(right);
        as_principal_wire(right)->oppsite = as_principal_wire(left);
        worker_connect_active_pair(self, as_principal_wire(left), as_principal_wire(right));
    } else if (is_wire(left)) {
        wire_t *wire = as_wire(left);
        if (wire->fuzed_value) {
            value_t fuzed_value = wire->fuzed_value;
            wire_destroy(&wire);
            worker_connect(self, fuzed_value, right);
        } else {
            wire->fuzed_value = right;
        }
    } else if (is_wire(right)) {
        wire_t *wire = as_wire(right);
        if (wire->fuzed_value) {
            value_t fuzed_value = wire->fuzed_value;
            wire_destroy(&wire);
            worker_connect(self, fuzed_value, left);
        } else {
            wire->fuzed_value = left;
        }
    } else {
        assert(false);
    }
}

void
worker_connect_top_value_pair(worker_t *self) {
    value_t second = stack_pop(self->value_stack);
    value_t first = stack_pop(self->value_stack);

    worker_connect(self, first, second);
}

void
worker_connect_active_pair(worker_t *self, principal_wire_t *left, principal_wire_t *right) {
    const rule_t *rule = mod_find_rule(self->mod, left, right);
    if (!rule) {
        // TODO should not lost the connection
        return;
    }

    worker_return_task(self, task_new(left, right, rule));
}

node_t *
worker_add_node(worker_t* self, const node_ctor_t *ctor) {
    return node_new_per_thread(self->node_allocator, self->free_node_stack, ctor);
}

void
worker_delete_node(worker_t* self, node_t *node) {
    node_recycle_per_thread(self->node_allocator, self->free_node_stack, &node);
}

void
worker_return_task(worker_t* self, task_t *task) {
    queue_back_push(self->task_queue, task);
}

node_t *
worker_lookup_node_by_wire(worker_t* self, wire_t *wire) {
    array_t *node_array = allocated_node_array(self->node_allocator);
    for (size_t i = 0; i < array_length(node_array); i++) {

        node_t *node = array_get(node_array, i);
        if (node_has_wire(node, wire))
            return node;
    }

    return NULL;
}
