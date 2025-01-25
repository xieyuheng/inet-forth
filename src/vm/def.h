#pragma once

typedef enum {
    PRIMITIVE_DEF,
    FUNCTION_DEF,
    CONSTANT_DEF,
    NODE_DEF,
} def_kind_t;

struct def_t {
    def_kind_t kind;
    union {
        primitive_def_t *primitive_def;
        function_def_t *function_def;
        constant_def_t *constant_def;
        node_ctor_t *node_ctor;
    };
};

def_t *def_from_primitive_def(primitive_def_t *primitive_def);
def_t *def_from_function_def(function_def_t *function_def);
def_t *def_from_constant_def(constant_def_t *constant_def);
def_t *def_from_node_ctor(node_ctor_t *node_ctor);

void def_destroy(def_t **self_pointer);

const char *def_name(const def_t *self);
const char *def_kind_name(def_kind_t kind);

void def_print(const def_t *self, file_t *file);
