#pragma once

typedef enum {
    OPCODE_APPLY,
    OPCODE_LITERAL,
    OPCODE_GET_VARIABLE,
    OPCODE_SET_VARIABLE,
} opcode_kind_t;

struct opcode_t {
    opcode_kind_t kind;
    union {
        struct { size_t arity; } apply;
        struct { value_t value; } literal;
        struct { size_t index; } get_variable;
        struct { size_t index; } set_variable;
    };
};

opcode_t *opcode_apply(size_t arity);
opcode_t *opcode_literal(value_t value);
opcode_t *opcode_get_variable(size_t index);
opcode_t *opcode_put_variable(size_t index);

void opcode_destroy(opcode_t **self_pointer);

void opcode_print(const opcode_t *self, file_t *file);
