#include "index.h"

opcode_t *
opcode_apply(size_t arity) {
    opcode_t *self = new(opcode_t);
    self->kind = OPCODE_APPLY;
    self->apply.arity = arity;
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
    case OPCODE_APPLY: {
        break;
    }

    case OPCODE_LITERAL: {
        break;
    }

    case OPCODE_GET_VARIABLE: {
        break;
    }

    case OPCODE_SET_VARIABLE: {
        break;
    }
    }

    free(self);
    *self_pointer = NULL;
    return;
}

void
opcode_print(const opcode_t *self, file_t *file) {
    switch (self->kind) {
    case OPCODE_APPLY: {
        fprintf(file, "(apply %lu)", self->apply.arity);
        return;
    }

    case OPCODE_LITERAL: {
        fprintf(file, "(literal ");
        value_print(self->literal.value, file);
        fprintf(file, ")");
        return;
    }

    case OPCODE_GET_VARIABLE: {
        fprintf(file, "(get-variable %ld)", self->get_variable.index);
        return;
    }

    case OPCODE_SET_VARIABLE: {
        fprintf(file, "(set-variable %ld)", self->set_variable.index);
        return;
    }
    }
}
