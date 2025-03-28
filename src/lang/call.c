#include "index.h"

void
call(worker_t *worker, const def_t *def) {
    switch (def->kind) {
    case DEF_PRIMITIVE: {
        call_primitive(worker, def->primitive);
        return;
    }

    case DEF_FUNCTION: {
        frame_t *frame = frame_new(def->function_def->function);
        stack_push(worker->return_stack, frame);
        return;
    }

    case DEF_CONSTANT: {
        stack_push(worker->value_stack, def->constant.value);
        return;
    }

    case DEF_NODE_CTOR: {
        call_node_ctor(worker, def->node_ctor);
        return;
    }
    }
}
