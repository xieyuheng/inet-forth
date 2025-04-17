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
    self->task_deque = deque_new();
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
    deque_destroy(&self->task_deque);
    stack_destroy(&self->value_stack);
    stack_destroy(&self->return_stack);
    free(self);
    *self_pointer = NULL;
}

atomic_size_t atomic_node_count = 0;

node_t *
worker_new_node(worker_t* self, const node_ctor_t *ctor) {
#if DEBUG_NODE_ALLOCATOR_DISABLED
    (void) self;
    node_t *node = node_new();
    node->ctor = ctor;
    node->id = atomic_add1(&atomic_node_count);
    return node;
#else
    node_t *node = node_allocator_allocate(self->node_allocator, self->free_node_stack);
    node->ctor = ctor;
    return node;
#endif
}

void
worker_recycle_node(worker_t* self, node_t *node) {
#if DEBUG_NODE_ALLOCATOR_DISABLED
    (void) self;
    node_clean(node);

    // NOTE To exclude ABA problem
    // we do not even free the memory of node.
    // node_destroy(&node);
#else
    node_clean(node);
    node_allocator_recycle(self->node_allocator, self->free_node_stack, &node);
#endif
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

    // size_t task_queue_length = queue_length(self->task_queue);
    // fprintf(file, "<task-queue length=\"%lu\">\n", task_queue_length);
    // for (size_t i = 0; i < task_queue_length; i++) {
    //     task_t *task = queue_get(self->task_queue, i);
    //     task_print(task, file);
    //     fprintf(file, "\n");
    // }
    // fprintf(file, "</task-queue>\n");

    worker_print_return_stack(self, file);
    worker_print_value_stack(self, file);

    fprintf(file, "</worker>\n");
}
