#include "index.h"

void
call(vm_t *vm, const def_t *def) {
    switch (def->kind) {
    case PRIMITIVE_DEF: {
        call_primitive(vm, def->primitive_def);
        return;
    }

    case FUNCTION_DEF: {
        call_function(vm, def->function_def);
        return;
    }

    case CONSTANT_DEF: {
        call_constant(vm, def->constant_def);
        return;
    }

    case NODE_DEF: {
        call_node(vm, def->node_ctor);
        return;
    }
    }
}
