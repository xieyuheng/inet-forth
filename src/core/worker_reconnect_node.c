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

    release_store(&node->atomic_is_ready, true);

#if DEBUG_NODE_LOCK
    mutex_unlock(node->mutex);
#else
    mutex_unlock(node->mutex);
#endif

    // NOTE We might have data race here!

    // `worker_disconnect_node` reports
    // lock contention with the lock above.

    // Building of this node can be synchronized
    // with the worker thread that get the task
    // (via the queue).

    // But building of the oppsite node of this node,
    // which might happen in another worker thread at "the same" time,
    // can NOT be synchronized by this `release_store`.

    // We can verify this by
    //     #define DEBUG_NODE_LOCK 0
    // and
    //     assert(acquire_load(&task->left->node->atomic_is_ready));
    //     assert(acquire_load(&task->right->node->atomic_is_ready));
    // in `step_task`, and seeing
    // only the second `assert` will fail.

    // Due to this reason a lock in node is actually required,
    // the lock contention reported by `worker_disconnect_node`
    /// is not a bug.

    if (found_task) {
        worker_add_task(worker, found_task);
    }
}
