#pragma once

typedef enum {
    DEF_PRIMITIVE,
    DEF_FUNCTION,
    DEF_NODE_CTOR,
} def_kind_t;

struct def_t {
    def_kind_t kind;
    union {
        primitive_t *primitive;
        function_t *function;
        node_ctor_t *node_ctor;
    };
};

def_t *def_primitive(primitive_t *primitive);
def_t *def_function(function_t *function);
def_t *def_node_ctor(node_ctor_t *node_ctor);

void def_destroy(def_t **self_pointer);

const char *def_name(const def_t *self);
const char *def_kind_name(def_kind_t kind);

void def_print(const def_t *self, file_t *file);
