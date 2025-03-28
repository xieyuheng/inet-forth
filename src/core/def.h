#pragma once

typedef enum {
    DEF_PRIMITIVE,
    DEF_FUNCTION,
    DEF_CONSTANT,
    DEF_NODE,
} def_kind_t;

struct def_t {
    def_kind_t kind;
    union {
        primitive_t *primitive;
        function_def_t *function_def;
        constant_def_t *constant_def;
        node_ctor_t *node_ctor;
    };
};

def_t *def_primitive(primitive_t *primitive);
def_t *def_function(function_def_t *function_def);
def_t *def_constant(constant_def_t *constant_def);
def_t *def_node(node_ctor_t *node_ctor);

void def_destroy(def_t **self_pointer);

const char *def_name(const def_t *self);
const char *def_kind_name(def_kind_t kind);

void def_print(const def_t *self, file_t *file);
