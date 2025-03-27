#include "index.h"

void
call(worker_t *worker, const def_t *def) {
    switch (def->kind) {
    case PRIMITIVE_DEF: {
        call_primitive(worker, def->primitive_def);
        return;
    }

    case FUNCTION_DEF: {
        call_function(worker, def->function_def);
        return;
    }

    case CONSTANT_DEF: {
        call_constant(worker, def->constant_def);
        return;
    }

    case NODE_DEF: {
        call_node(worker, def->node_ctor);
        return;
    }
    }
}
