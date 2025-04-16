#include "index.h"

void
call_node_ctor(worker_t *worker, const node_ctor_t *ctor) {
    node_t *node = worker_add_node(worker, ctor);
    worker_apply_node(worker, node);
}
