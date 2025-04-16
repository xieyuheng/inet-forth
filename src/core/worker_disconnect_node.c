#include "index.h"

void
worker_disconnect_node(worker_t *worker, node_t *node) {
    // mutex_lock(node->mutex);

    while (!mutex_try_lock(node->mutex)) {
        file_lock(stdout);
        test_printf("data race\n");
        test_printf("node: ");
        node_print(node, stdout);
        printf("\n");
        file_unlock(stdout);
    }

    for (size_t i = 0; i < node->ctor->arity; i++) {
        value_t value = node_get_value(node, i);
        if (is_principal_wire(value)) {
            principal_wire_t *principal_wire = as_principal_wire(value);
            principal_wire_destroy(&principal_wire);
        } else {
            stack_push(worker->value_stack, value);
        }
    }

    worker_delete_node(worker, node);

    mutex_unlock(node->mutex);
}
