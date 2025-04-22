#pragma once

extern object_spec_t function_object_spec;

struct function_t {
    object_spec_t *spec;
    char *name;
    hash_t *local_index_hash;
    array_t *opcode_array;
};

function_t *function_new(void);
void function_destroy(function_t **self_pointer);

bool is_function(value_t value);
function_t *as_function(value_t value);

size_t function_length(const function_t *self);

void function_add_opcode(function_t *self, opcode_t *opcode);
opcode_t *function_get_opcode(const function_t *self, size_t index);

void function_print_name(const function_t *self, file_t *file);
void function_print(const function_t *self, file_t *file);
void function_print_with_cursor(const function_t *self, file_t *file, size_t cursor);
