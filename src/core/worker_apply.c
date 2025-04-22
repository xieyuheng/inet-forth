#include "index.h"

static void
worker_apply_primitive(worker_t *worker, const primitive_t *primitive) {
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
worker_apply(worker_t *worker, value_t target, size_t arity) {
    (void) arity;
    if (is_node_ctor(target)) {
        node_t *node = worker_new_node(worker, as_node_ctor(target));
        worker_reconnect_node(worker, node);
    } else if (is_function(target)) {
        assert(as_function(target)->arity == 0);
        frame_t *frame = frame_new(as_function(target));
        stack_push(worker->return_stack, frame);
    } else if (is_primitive(target)) {
        worker_apply_primitive(worker, as_primitive(target));
    } else {
        who_printf("unknown target: ");
        value_print(target, stdout);
        printf("\n");
        exit(1);
    }
}
