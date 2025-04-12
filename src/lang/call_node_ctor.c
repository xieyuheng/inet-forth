#include "index.h"

static void
node_take_input(worker_t *worker, node_t *node, size_t index, value_t value) {
    node_set_value(node, index, value);

    if (is_wire(value)) {
        wire_t *wire = as_wire(value);
        if (wire_is_principal(wire))
            maybe_return_task(worker, wire);
    }
}

static value_t
node_return_output(worker_t *worker, node_t *node, size_t index) {
    (void) worker;

    wire_t *node_wire = wire_new();
    wire_t *free_wire = wire_new();

    node_set_value(node, index, node_wire);

    wire_set_opposite(node_wire, free_wire);
    wire_set_opposite(free_wire, node_wire);

    return free_wire;
}

void
call_node_ctor(worker_t *worker, const node_ctor_t *ctor) {
    node_t *node = worker_add_node(worker, ctor);

    for (size_t count = 0; count < node->ctor->input_arity; count++) {
        size_t index = node->ctor->input_arity - 1 - count;
        value_t value = stack_pop(worker->value_stack);
        node_take_input(worker, node, index, value);
    }

    for (size_t count = 0; count < node->ctor->output_arity; count++) {
        size_t index = node->ctor->input_arity + count;
        value_t value  = node_return_output(worker, node, index);
        stack_push(worker->value_stack, value);
    }

    return;
}
