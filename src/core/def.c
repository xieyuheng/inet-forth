#include "index.h"

def_t *
def_from_primitive_def(primitive_def_t *primitive_def) {
    def_t *self = new(def_t);
    self->kind = PRIMITIVE_DEF;
    self->primitive_def = primitive_def;
    return self;
}

def_t *
def_from_function_def(function_def_t *function_def) {
    def_t *self = new(def_t);
    self->kind = FUNCTION_DEF;
    self->function_def = function_def;
    return self;
}

def_t *
def_from_constant_def(constant_def_t *constant_def) {
    def_t *self = new(def_t);
    self->kind = CONSTANT_DEF;
    self->constant_def = constant_def;
    return self;
}

def_t *
def_from_node_ctor(node_ctor_t *node_ctor) {
    def_t *self = new(def_t);
    self->kind = NODE_DEF;
    self->node_ctor = node_ctor;
    return self;
}


void
def_destroy(def_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        def_t *self = *self_pointer;

        switch (self->kind) {
        case PRIMITIVE_DEF: {
            primitive_def_destroy(&self->primitive_def);
            break;
        }

        case FUNCTION_DEF: {
            function_def_destroy(&self->function_def);
            break;
        }

        case CONSTANT_DEF: {
            constant_def_destroy(&self->constant_def);
            break;
        }

        case NODE_DEF: {
            node_ctor_destroy(&self->node_ctor);
            break;
        }
        }

        free(self);
        *self_pointer = NULL;
    }
}

const char *
def_name(const def_t *def) {
    switch (def->kind) {
    case PRIMITIVE_DEF: {
        return def->primitive_def->name;
    }

    case FUNCTION_DEF: {
        return def->function_def->name;
    }

    case CONSTANT_DEF: {
        return def->constant_def->name;
    }

    case NODE_DEF: {
        return def->node_ctor->name;
    }
    }

    assert(false);
}

const char *
def_kind_name(def_kind_t kind) {
    switch (kind) {
    case PRIMITIVE_DEF: {
        return "primitive";
    }

    case FUNCTION_DEF: {
        return "function";
    }

    case CONSTANT_DEF: {
        return "constant";
    }

    case NODE_DEF: {
        return "node";
    }
    }

    assert(false);
}

void
def_print(const def_t *def, file_t *file) {
    switch (def->kind) {
    case PRIMITIVE_DEF: {
        fprintf(file, "define-primitive %s", def->primitive_def->name);
        return;
    }

    case FUNCTION_DEF: {
        fprintf(file, "define %s ", def->function_def->name);
        function_print(def->function_def->function, file);
        return;
    }

    case CONSTANT_DEF: {
        fprintf(file, "define-constant %s ", def->constant_def->name);
        return;
    }

    case NODE_DEF: {
        fprintf(file, "define-node %s ", def->node_ctor->name);
        for (size_t i = 0; i < def->node_ctor->input_arity; i++) {
            port_info_t *port_info = def->node_ctor->port_infos[i];
            if (port_info->is_principal) {
                fprintf(file, "%s! ", port_info->name);
            } else {
                fprintf(file, "%s ", port_info->name);
            }
        }

        fprintf(file, "-- ");

        for (size_t c = 0; c < def->node_ctor->output_arity; c++) {
            size_t i = def->node_ctor->input_arity + c;
            port_info_t *port_info = def->node_ctor->port_infos[i];
            if (port_info->is_principal) {
                fprintf(file, "%s! ", port_info->name);
            } else {
                fprintf(file, "%s ", port_info->name);
            }
        }

        return;
    }
    }

    assert(false);
}
