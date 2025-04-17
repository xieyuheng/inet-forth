#include "index.h"

void
worker_disconnect_node(worker_t *worker, node_t *node) {
#if DEBUG_NODE_MUTEX
    mutex_t *mutex = node->mutex;
    while (!mutex_try_lock(mutex)) {
        file_lock(stdout);
        test_printf("lock contention! ");
        printf("worker #%lu, ", worker->index);
        printf("locked by #%lu, ", ((worker_t *) node->locked_by_worker)->index);
        printf("node: "); node_print(node, stdout);
        printf("\n");
        file_unlock(stdout);
    }
#endif

    assert(acquire_load(&node->atomic_is_ready));

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

#if DEBUG_NODE_MUTEX && DEBUG_NODE_ALLOCATOR_DISABLED
    mutex_unlock(mutex);
    mutex_destroy(&mutex);
#elif DEBUG_NODE_MUTEX
    mutex_unlock(mutex);
#endif
}
