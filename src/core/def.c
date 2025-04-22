#include "index.h"

def_t *
def_primitive(primitive_t *primitive) {
    def_t *self = new(def_t);
    self->kind = DEF_PRIMITIVE;
    self->primitive = primitive;
    return self;
}

def_t *
def_function(function_t *function) {
    def_t *self = new(def_t);
    self->kind = DEF_FUNCTION;
    self->function = function;
    return self;
}

def_t *
def_node_ctor(node_ctor_t *node_ctor) {
    def_t *self = new(def_t);
    self->kind = DEF_NODE_CTOR;
    self->node_ctor = node_ctor;
    return self;
}


void
def_destroy(def_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    def_t *self = *self_pointer;

    switch (self->kind) {
    case DEF_PRIMITIVE: {
        primitive_destroy(&self->primitive);
        break;
    }

    case DEF_FUNCTION: {
        function_destroy(&self->function);
        break;
    }

    case DEF_NODE_CTOR: {
        node_ctor_destroy(&self->node_ctor);
        break;
    }
    }

    free(self);
    *self_pointer = NULL;
}

const char *
def_name(const def_t *def) {
    switch (def->kind) {
    case DEF_PRIMITIVE: {
        return def->primitive->name;
    }

    case DEF_FUNCTION: {
        return def->function->name;
    }

    case DEF_NODE_CTOR: {
        return def->node_ctor->name;
    }
    }

    assert(false);
}

const char *
def_kind_name(def_kind_t kind) {
    switch (kind) {
    case DEF_PRIMITIVE: {
        return "primitive";
    }

    case DEF_FUNCTION: {
        return "function";
    }

    case DEF_NODE_CTOR: {
        return "node";
    }
    }

    assert(false);
}

void
def_print(const def_t *def, file_t *file) {
    switch (def->kind) {
    case DEF_PRIMITIVE: {
        fprintf(file, "define-primitive %s", def->primitive->name);
        return;
    }

    case DEF_FUNCTION: {
        fprintf(file, "define %s ", def->function->name);
        function_print(def->function, file);
        return;
    }

    case DEF_NODE_CTOR: {
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
