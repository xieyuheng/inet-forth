#include "index.h"

opcode_t *
opcode_call(const def_t *def) {
    opcode_t *self = new(opcode_t);
    self->kind = OPCODE_CALL;
    self->call.def = def;
    return self;
}

opcode_t *
opcode_literal(value_t value) {
    opcode_t *self = new(opcode_t);
    self->kind = OPCODE_LITERAL;
    self->literal.value = value;
    return self;
}


opcode_t *
opcode_get_variable(size_t index) {
    opcode_t *self = new(opcode_t);
    self->kind = OPCODE_GET_VARIABLE;
    self->get_variable.index = index;
    return self;
}

opcode_t *
opcode_set_variable(size_t index) {
    opcode_t *self = new(opcode_t);
    self->kind = OPCODE_SET_VARIABLE;
    self->set_variable.index = index;
    return self;
}

void
opcode_destroy(opcode_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    opcode_t *self = *self_pointer;
    switch (self->kind) {
    case OPCODE_CALL: {
        free(self);
        *self_pointer = NULL;
        return;
    }

    case OPCODE_LITERAL: {
        free(self);
        *self_pointer = NULL;
        return;
    }

    case OPCODE_GET_VARIABLE: {
        free(self);
        *self_pointer = NULL;
        return;
    }

    case OPCODE_SET_VARIABLE: {
        free(self);
        *self_pointer = NULL;
        return;
    }
    }
}

void
opcode_print(const opcode_t *op, file_t *file) {
    switch (op->kind) {
    case OPCODE_CALL: {
        fprintf(file, "CALL %s", def_name(op->call.def));
        return;
    }

    case OPCODE_LITERAL: {
        fprintf(file, "LITERAL ");
        value_print(op->literal.value, file);
        return;
    }

    case OPCODE_GET_VARIABLE: {
        fprintf(file, "LOCAL-GET %ld", op->get_variable.index);
        return;
    }

    case OPCODE_SET_VARIABLE: {
        fprintf(file, "LOCAL-SET %ld", op->set_variable.index);
        return;
    }
    }

    assert(false);
}
