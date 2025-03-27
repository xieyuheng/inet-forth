#include "index.h"

void x_ok(worker_t *worker) {
    bool ok = to_bool(stack_pop(worker->value_stack));
    if (!ok) {
        fprintf(stderr, "[assertion-failed]\n");
        worker_print(worker, stderr);
        exit(1);
    }
}
