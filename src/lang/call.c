#include "index.h"

void
call(worker_t *worker, const def_t *def) {
    switch (def->kind) {
    case DEF_PRIMITIVE: {
        call_primitive(worker, def->primitive);
        return;
    }

    case DEF_FUNCTION: {
        call_function(worker, def->function_def);
        return;
    }

    case DEF_CONSTANT: {
        call_constant(worker, def->constant_def);
        return;
    }

    case DEF_NODE: {
        call_node(worker, def->node_ctor);
        return;
    }
    }
}
