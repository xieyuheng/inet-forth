#include "index.h"

static void
worker_call_primitive(worker_t *worker, const primitive_t *primitive) {
    switch (primitive->fn_kind) {
    case PRIMITIVE_FN: {
        primitive->primitive_fn(worker);
        return;
    }

    case PRIMITIVE_FN_0: {
        value_t result = primitive->primitive_fn_0();
        if (result != NULL)
            stack_push(worker->value_stack, result);
        return;
    }

    case PRIMITIVE_FN_1: {
        value_t x = stack_pop(worker->value_stack);
        value_t result = primitive->primitive_fn_1(x);
        if (result != NULL)
            stack_push(worker->value_stack, result);
        return;
    }

    case PRIMITIVE_FN_2: {
        value_t y = stack_pop(worker->value_stack);
        value_t x = stack_pop(worker->value_stack);
        value_t result = primitive->primitive_fn_2(x, y);
        if (result != NULL)
            stack_push(worker->value_stack, result);
        return;
    }

    case PRIMITIVE_FN_3: {
        value_t z = stack_pop(worker->value_stack);
        value_t y = stack_pop(worker->value_stack);
        value_t x = stack_pop(worker->value_stack);
        value_t result = primitive->primitive_fn_3(x, y, z);
        if (result != NULL)
            stack_push(worker->value_stack, result);
        return;
    }

    case PRIMITIVE_FN_4: {
        value_t w = stack_pop(worker->value_stack);
        value_t z = stack_pop(worker->value_stack);
        value_t y = stack_pop(worker->value_stack);
        value_t x = stack_pop(worker->value_stack);
        value_t result = primitive->primitive_fn_4(x, y, z, w);
        if (result != NULL)
            stack_push(worker->value_stack, result);
        return;
    }
    }
}

void
worker_call(worker_t *worker, const def_t *def) {
    switch (def->kind) {
    case DEF_PRIMITIVE: {
        worker_call_primitive(worker, def->primitive);
        return;
    }

    case DEF_FUNCTION: {
        frame_t *frame = frame_new(def->function);
        stack_push(worker->return_stack, frame);
        return;
    }

    case DEF_CONSTANT: {
        stack_push(worker->value_stack, def->constant.value);
        return;
    }

    case DEF_NODE_CTOR: {
        node_t *node = worker_new_node(worker, def->node_ctor);
        worker_reconnect_node(worker, node);
        return;
    }
    }
}
