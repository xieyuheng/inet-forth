#include "index.h"

inline static task_t *
reconnect_input(node_t *node, size_t index, value_t value) {
    port_info_t *port_info = node_get_port_info(node, index);
    if (port_info->is_principal) {
        principal_wire_t *principal_wire = principal_wire_new(node, index);
        node_put_value(node, index, principal_wire);
        return connect(principal_wire, value);
    } else if (is_principal_wire(value)) {
        wire_t *wire = wire_new();
        node_put_value(node, index, wire);
        return connect(wire, value);
    } else {
        node_put_value(node, index, value);
        return NULL;
    }
}

inline static value_t
reconnect_output(node_t *node, size_t index) {
    port_info_t *port_info = node_get_port_info(node, index);
    if (port_info->is_principal) {
        principal_wire_t *principal_wire = principal_wire_new(node, index);
        node_put_value(node, index, principal_wire);
        return principal_wire;
    } else {
        wire_t *wire = wire_new();
        node_put_value(node, index, wire);
        return wire;
    }
}

void
worker_reconnect_node(worker_t *worker, node_t *node) {
    // NOTE We might have data race here,
    // Because although adding `found_task` to the worker's queue
    // will synchronize the code that build this node
    // with the worker thread that get the task,
    // but the code that build the oppsite node of this node
    // will not be synchronized with adding `found_task`.
    // Due to this reason a lock in node is actually required.

#if DEBUG_NODE_LOCK
    while (!mutex_try_lock(node->mutex)) {
        file_lock(stdout);
        who_printf("lock contention! ");
        who_printf("node: "); node_print(node, stdout); printf("\n");
        file_unlock(stdout);
    }

    node->locked_by_worker = worker;
#else
    mutex_lock(node->mutex);
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

#if DEBUG_NODE_LOCK
    mutex_unlock(node->mutex);
#else
    mutex_unlock(node->mutex);
#endif

    if (found_task) {
        worker_add_task(worker, found_task);
    }
}
