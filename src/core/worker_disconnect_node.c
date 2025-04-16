#include "index.h"

void
worker_disconnect_node(worker_t *worker, node_t *node) {
#if DEBUG_NODE_MUTEX
    while (!mutex_try_lock(node->mutex)) {
        file_lock(stdout);
        test_printf("data race! ");
        printf("worker #%lu, ", worker->index);
        printf("node: "); node_print(node, stdout);
        printf("\n");
        file_unlock(stdout);
    }
#endif

    for (size_t i = 0; i < node->ctor->arity; i++) {
        value_t value = node_get_value(node, i);
        if (is_principal_wire(value)) {
            principal_wire_t *principal_wire = as_principal_wire(value);
            principal_wire_destroy(&principal_wire);
        } else {
            stack_push(worker->value_stack, value);
        }
    }

    worker_recycle_node(worker, node);

#if DEBUG_NODE_MUTEX
    mutex_unlock(node->mutex);
#endif
}
