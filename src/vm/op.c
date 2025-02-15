#include "index.h"

op_t *
op_call(const def_t *def) {
    op_t *self = new(op_t);
    self->kind = OP_CALL;
    self->call.def = def;
    return self;
}

op_t *
op_literal(value_t value) {
    op_t *self = new(op_t);
    self->kind = OP_LITERAL;
    self->literal.value = value;
    return self;
}


op_t *
op_get_variable(size_t index) {
    op_t *self = new(op_t);
    self->kind = OP_GET_VARIABLE;
    self->get_variable.index = index;
    return self;
}

op_t *
op_set_variable(size_t index) {
    op_t *self = new(op_t);
    self->kind = OP_SET_VARIABLE;
    self->set_variable.index = index;
    return self;
}

void
op_destroy(op_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        op_t *self = *self_pointer;
        switch (self->kind) {
        case OP_CALL: {
            free(self);
            *self_pointer = NULL;
            return;
        }

        case OP_LITERAL: {
            free(self);
            *self_pointer = NULL;
            return;
        }

        case OP_GET_VARIABLE: {
            free(self);
            *self_pointer = NULL;
            return;
        }

        case OP_SET_VARIABLE: {
            free(self);
            *self_pointer = NULL;
            return;
        }
        }
    }
}

void
op_print(const op_t *op, file_t *file) {
    switch (op->kind) {
    case OP_CALL: {
        fprintf(file, "CALL %s", def_name(op->call.def));
        return;
    }

    case OP_LITERAL: {
        fprintf(file, "LITERAL ");
        value_print(op->literal.value, file);
        return;
    }

    case OP_GET_VARIABLE: {
        fprintf(file, "LOCAL-GET %ld", op->get_variable.index);
        return;
    }

    case OP_SET_VARIABLE: {
        fprintf(file, "LOCAL-SET %ld", op->set_variable.index);
        return;
    }
    }

    assert(false);
}
