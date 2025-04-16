#include "index.h"

static void
node_take_input(worker_t *worker, node_t *node, size_t index, value_t value) {
    port_info_t *port_info = node_get_port_info(node, index);
    if (port_info->is_principal) {
        principal_wire_t *principal_wire = principal_wire_new(node, index);
        node_set_value(node, index, principal_wire);
        worker_connect(worker, principal_wire, value);
    } else if (is_principal_wire(value)) {
        wire_t *wire = wire_new();
        node_set_value(node, index, wire);
        worker_connect(worker, wire, value);
    } else {
        node_set_value(node, index, value);
    }
}

static value_t
node_return_output(worker_t *worker, node_t *node, size_t index) {
    (void) worker;

    port_info_t *port_info = node_get_port_info(node, index);
    if (port_info->is_principal) {
        principal_wire_t *principal_wire = principal_wire_new(node, index);
        node_set_value(node, index, principal_wire);
        return principal_wire;
    } else {
        wire_t *wire = wire_new();
        node_set_value(node, index, wire);
        return wire;
    }
}

void
worker_apply_node(worker_t *worker, node_t *node) {
    // mutex_lock(node->mutex);

    while (!mutex_try_lock(node->mutex)) {
        file_lock(stdout);
        test_printf("data race\n");
        test_printf("node: ");
        node_print(node, stdout);
        printf("\n");
        file_unlock(stdout);
    }

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

    mutex_unlock(node->mutex);
}
