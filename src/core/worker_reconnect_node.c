#include "index.h"

inline static task_t *
reconnect_input(node_t *node, size_t index, value_t value) {
    port_info_t *port_info = node_get_port_info(node, index);
    if (port_info->is_principal) {
        principal_wire_t *principal_wire = principal_wire_new(node, index);
        node_set_value(node, index, principal_wire);
        return connect(principal_wire, value);
    } else if (is_principal_wire(value)) {
        wire_t *wire = wire_new();
        node_set_value(node, index, wire);
        return connect(wire, value);
    } else {
        node_set_value(node, index, value);
        return NULL;
    }
}

inline static value_t
reconnect_output(node_t *node, size_t index) {
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
        test_printf("node: "); node_print(node, stdout); printf("\n");
        file_unlock(stdout);
    }
#endif

    task_t *found_task = NULL;
    for (size_t count = 0; count < node->ctor->input_arity; count++) {
        size_t index = node->ctor->input_arity - 1 - count;
        value_t value = stack_pop(worker->value_stack);
        task_t *task = reconnect_input(node, index, value);
        if (task) {
            assert(!found_task);
            found_task = task;
        }
    }

    for (size_t count = 0; count < node->ctor->output_arity; count++) {
        size_t index = node->ctor->input_arity + count;
        value_t value  = reconnect_output(node, index);
        stack_push(worker->value_stack, value);
    }

    // NOTE We must add task at the END
    // to avoid data race during work stealing.
    // TODO still have data race :(
    if (found_task) {
        file_lock(stdout);
        test_printf("add task to worker #%lu start\n", worker->index);
        test_printf("node: "); node_print(node, stdout); printf("\n");
        test_printf("task: "); task_print(found_task, stdout); printf("\n");
        worker_add_task(worker, found_task);
        test_printf("add task to worker #%lu finished\n", worker->index);
        file_unlock(stdout);
    }

#if DEBUG_NODE_MUTEX
    mutex_unlock(node->mutex);
#endif
}
