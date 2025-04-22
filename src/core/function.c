#include "index.h"

function_t *
function_new(void) {
    function_t *self = new(function_t);
    self->local_index_hash = hash_of_string_key();
    self->opcode_array = array_new_auto_with((destroy_fn_t *) opcode_destroy);
    return self;
}

void
function_destroy(function_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    function_t *self = *self_pointer;
    string_destroy(&self->name);
    hash_destroy(&self->local_index_hash);
    array_destroy(&self->opcode_array);
    free(self);
    *self_pointer = NULL;
}

size_t
function_length(const function_t *self) {
    return array_length(self->opcode_array);
}

void
function_add_op(function_t *self, opcode_t *op) {
    array_push(self->opcode_array, op);
}

opcode_t *
function_get_op(const function_t *self, size_t index) {
    return array_get(self->opcode_array, index);
}

void
function_print(const function_t *self, file_t *file) {
    for (size_t i = 0; i < function_length(self); i++) {
        opcode_print(function_get_op(self, i), file);
        fprintf(file, "\n");
    }
}

void
function_print_with_cursor(const function_t *self, file_t *file, size_t cursor) {
    for (size_t i = 0; i < function_length(self); i++) {
        if (i == cursor) {
            opcode_print(function_get_op(self, i), file);
            fprintf(file, " <<<");
            fprintf(file, "\n");
        } else {
            opcode_print(function_get_op(self, i), file);
            fprintf(file, "\n");
        }
    }
}
