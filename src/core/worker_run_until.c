#include "index.h"

inline static void
worker_execute_opcode(worker_t *worker, frame_t *frame, opcode_t *opcode) {
    switch (opcode->kind) {
    case OPCODE_APPLY: {
        value_t target = stack_pop(worker->value_stack);
        worker_apply(worker, target, opcode->apply.arity);
        return;
    }

    case OPCODE_LITERAL: {
        stack_push(worker->value_stack, opcode->literal.value);
        return;
    }

    case OPCODE_GET_VARIABLE: {
        value_t value = frame_get_variable(frame, opcode->get_variable.index);
        stack_push(worker->value_stack, value);
        return;
    }

    case OPCODE_SET_VARIABLE: {
        value_t value = stack_pop(worker->value_stack);
        frame_set_variable(frame, opcode->set_variable.index, value);
        return;
    }
    }
}

inline static void
worker_run_one_step(worker_t *worker) {
    if (stack_is_empty(worker->return_stack)) return;

    frame_t *frame = stack_pop(worker->return_stack);
    if (frame_is_finished(frame)) {
        frame_destroy(&frame);
        return;
    }

    opcode_t *op = frame_fetch_opcode(frame);

    // proper tail-call = do not push finished frame.
    bool finished = frame_is_finished(frame);
    if (!finished) {
        stack_push(worker->return_stack, frame);
    }

    worker_execute_opcode(worker, frame, op);

    if (finished) {
        frame_destroy(&frame);
    }

#if DEBUG_STEP_LOG
    worker_print(worker, stdout);
    fprintf(stdout, "\n");
#endif
}

void
worker_run_until(worker_t *worker, size_t base_length) {
    while (stack_length(worker->return_stack) > base_length) {
        worker_run_one_step(worker);
    }
}
