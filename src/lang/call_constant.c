#include "index.h"

void
call_constant(worker_t *worker, const constant_def_t *def) {
    stack_push(worker->value_stack, def->value);
    return;
}
