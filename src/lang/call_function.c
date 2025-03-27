#include "index.h"

void
call_function(worker_t *worker, const function_def_t *def) {
    frame_t *frame = frame_new(def->function);
    stack_push(worker->return_stack, frame);
    return;
}
