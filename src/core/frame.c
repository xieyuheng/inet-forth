#include "index.h"

#define VARIABLE_ARRAY_SIZE 64

struct frame_t {
    size_t cursor;
    const function_t *function;
    array_t *variable_array;
};

frame_t *
frame_new(const function_t *function) {
    frame_t *self = new(frame_t);
    self->cursor = 0;
    self->function = function;
    self->variable_array = array_new(VARIABLE_ARRAY_SIZE);
    return self;
}

void
frame_destroy(frame_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

     frame_t *self = *self_pointer;
     // does not own function
     array_destroy(&self->variable_array);
     free(self);
     *self_pointer = NULL;
}

bool
frame_is_finished(const frame_t *self) {
    return self->cursor == function_length(self->function);
}

opcode_t *
frame_fetch_opcode(frame_t *self) {
    opcode_t *opcode = function_get_op(self->function, self->cursor);
    self->cursor++;
    return opcode;
}

void
frame_print(const frame_t *self, file_t *file) {
    fprintf(file, "<frame>\n");

    fprintf(file, "<function>\n");
    function_print_with_cursor(self->function, file, self->cursor);
    fprintf(file, "</function>\n");

    fprintf(file, "</frame>\n");
}

value_t
frame_get_variable(const frame_t *self, size_t index) {
    return array_get(self->variable_array, index);
}

void
frame_set_variable(frame_t *self, size_t index, value_t value) {
    array_set(self->variable_array, index, value);
}
