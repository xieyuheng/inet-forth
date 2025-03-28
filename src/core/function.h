#pragma once

struct function_t {
    hash_t *local_index_hash;
    list_t *op_list;
    size_t length;
    op_t **ops;
};

function_t *function_new(void);
void function_destroy(function_t **self_pointer);

size_t function_length(const function_t *self);

void function_add_op(function_t *self, op_t *op);
void function_build(function_t *self);
op_t *function_get_op(const function_t *self, size_t index);

void function_print(const function_t *self, file_t *file);
void function_print_with_cursor(const function_t *self, file_t *file, size_t cursor);
