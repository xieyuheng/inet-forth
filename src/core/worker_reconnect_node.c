#include "index.h"

inline static void
reconnect_input(worker_t *worker, node_t *node, size_t index, value_t value) {
    port_info_t *port_info = node_get_port_info(node, index);
    if (port_info->is_principal) {
        principal_wire_t *principal_wire = principal_wire_new(node, index);
        node_set_value(node, index, principal_wire);
        task_t *task = worker_connect(worker, principal_wire, value);
        if (task) {
            worker_add_task(worker, task);
        }
    } else if (is_principal_wire(value)) {
        wire_t *wire = wire_new();
        node_set_value(node, index, wire);
        task_t *task = worker_connect(worker, wire, value);
        if (task) {
            worker_add_task(worker, task);
        }
    } else {
        node_set_value(node, index, value);
    }
}

inline static value_t
reconnect_output(worker_t *worker, node_t *node, size_t index) {
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
worker_reconnect_node(worker_t *worker, node_t *node) {
#if DEBUG_NODE_MUTEX
    while (!mutex_try_lock(node->mutex)) {
        file_lock(stdout);
        test_printf("data race\n");
        test_printf("node: ");
        node_print(node, stdout);
        printf("\n");
        file_unlock(stdout);
    }
#endif

    for (size_t count = 0; count < node->ctor->input_arity; count++) {
        size_t index = node->ctor->input_arity - 1 - count;
        value_t value = stack_pop(worker->value_stack);
        reconnect_input(worker, node, index, value);
    }

    for (size_t count = 0; count < node->ctor->output_arity; count++) {
        size_t index = node->ctor->input_arity + count;
        value_t value  = reconnect_output(worker, node, index);
        stack_push(worker->value_stack, value);
    }

#if DEBUG_NODE_MUTEX
    mutex_unlock(node->mutex);
#endif
}
