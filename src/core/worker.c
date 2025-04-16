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
    self->task_queue_front_mutex = mutex_new();
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

static void
worker_return_task(worker_t *self, task_t *task) {
    queue_back_push(self->task_queue, task);
    if (self->scheduler) {
        atomic_add1(&self->scheduler->atomic_task_count);
    }
}


inline static task_t *
rule_match(const rule_t *rule, principal_wire_t *left, principal_wire_t *right) {
    if ((rule->left_node_ctor == left->node->ctor) &&
        (rule->right_node_ctor == right->node->ctor)) {
        return task_new(left, right, rule);
    } else if ((rule->left_node_ctor == right->node->ctor) &&
               (rule->right_node_ctor == left->node->ctor)) {
        return task_new(right, left, rule);
    } else {
        return NULL;
    }
}

static void
worker_connect_active_pair(worker_t *self, principal_wire_t *left, principal_wire_t *right) {
    as_principal_wire(left)->oppsite = as_principal_wire(right);
    as_principal_wire(right)->oppsite = as_principal_wire(left);

    for (size_t i = 0; i < left->node->ctor->arity; i++) {
        rule_t *rule = array_get(left->node->ctor->rule_array, i);
        task_t* task = rule_match(rule, left, right);
        if (task) {
            worker_return_task(self, task);
            return;
        }
    }

    for (size_t i = 0; i < right->node->ctor->arity; i++) {
        rule_t *rule = array_get(right->node->ctor->rule_array, i);
        task_t* task = rule_match(rule, left, right);
        if (task) {
            worker_return_task(self, task);
            return;
        }
    }
}

static void
worker_fuze(worker_t *self, wire_t *wire, value_t value) {
    value_t fuzed_value = NULL;
    if (!atomic_compare_exchange_strong(
            &wire->atomic_fuzed_value,
            &fuzed_value,
            value))
    {
        wire_destroy(&wire);
        worker_connect(self, fuzed_value, value);
    }
}

void
worker_connect(worker_t *self, value_t left, value_t right) {
    if (is_principal_wire(left) && is_principal_wire(right)) {
        worker_connect_active_pair(self, as_principal_wire(left), as_principal_wire(right));
    } else if (is_wire(left)) {
        worker_fuze(self, as_wire(left), right);
    } else if (is_wire(right)) {
        worker_fuze(self, as_wire(right), left);
    } else {
        assert(false);
    }
}

node_t *
worker_new_node(worker_t* self, const node_ctor_t *ctor) {
    node_t *node = node_allocator_allocate(self->node_allocator, self->free_node_stack);
    node->ctor = ctor;
    return node;
}

void
worker_delete_node(worker_t* self, node_t *node) {
    node->ctor = NULL;
    node_allocator_recycle(self->node_allocator, self->free_node_stack, &node);
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
worker_print(const worker_t *self, file_t *file) {
    fprintf(file, "<worker>\n");

    size_t task_queue_length = queue_length(self->task_queue);
    fprintf(file, "<task-queue length=\"%lu\">\n", task_queue_length);
    for (size_t i = 0; i < task_queue_length; i++) {
        task_t *task = queue_get(self->task_queue, i);
        task_print(task, file);
        fprintf(file, "\n");
    }
    fprintf(file, "</task-queue>\n");

    worker_print_return_stack(self, file);
    worker_print_value_stack(self, file);

    fprintf(file, "</worker>\n");
}
