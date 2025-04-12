#include "index.h"

static void
node_take_input(worker_t *worker, node_t *node, size_t index, value_t value) {
    port_info_t *port_info = node_get_port_info(node, index);
    if (port_info->is_principal) {
        principal_port_t *principal_port = principal_port_new(node, index);
        if (is_wire(value)) {
            wire_connect(as_wire(value), principal_port);
        } else if (is_principal_port(value)) {
            return_task(worker, as_principal_port(value), principal_port);
        } else {
            assert(false);
        }
    } else {
        node_set_value(node, index, value);
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
