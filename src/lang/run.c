#include "index.h"

static void
step_op(worker_t *worker, frame_t *frame, op_t *op) {
    switch (op->kind) {
    case OP_CALL: {
        call(worker, op->call.def);
        return;
    }

    case OP_LITERAL: {
        stack_push(worker->value_stack, op->literal.value);
        return;
    }

    case OP_GET_VARIABLE: {
        value_t value = frame_get_variable(frame, op->get_variable.index);
        stack_push(worker->value_stack, value);
        return;
    }

    case OP_SET_VARIABLE: {
        value_t value = stack_pop(worker->value_stack);
        frame_set_variable(frame, op->set_variable.index, value);
        return;
    }
    }
}

static void
step(worker_t *worker) {
    if (stack_is_empty(worker->return_stack)) return;

    frame_t *frame = stack_pop(worker->return_stack);
    if (frame_is_finished(frame)) {
        frame_destroy(&frame);
        return;
    }

    op_t *op = frame_fetch_op(frame);

    // proper tail-call = do not push finished frame.
    bool finished = frame_is_finished(frame);
    if (!finished) {
        stack_push(worker->return_stack, frame);
    }

    step_op(worker, frame, op);

    if (finished) {
        frame_destroy(&frame);
    }

#if DEBUG_STEP_LOG
    worker_print(worker, stdout);
    fprintf(stdout, "\n");
#endif
}

void
run_until(worker_t *worker, size_t base_length) {
    while (stack_length(worker->return_stack) > base_length) {
        step(worker);
    }
}

void
step_task(worker_t *worker, task_t *task) {
#if DEBUG_TASK_MUTEX
    mutex_t *mutex = task->mutex;
    mutex_lock(mutex);
#endif

#if DEBUG_TASK_LOG
    file_lock(stdout);
    test_printf("worker #%ld, ", worker->index);
    printf("task: ");
    task_print(task, stdout);
    printf("\n");
    file_unlock(stdout);
#endif

    worker_disconnect_node(worker, task->left->node);
    worker_disconnect_node(worker, task->right->node);

    size_t base_length = stack_length(worker->return_stack);
    frame_t *frame = frame_new(task->rule->function);
    task_destroy(&task);
    stack_push(worker->return_stack, frame);
    run_until(worker, base_length);

#if DEBUG_TASK_MUTEX
    mutex_unlock(mutex);
    mutex_destroy(&mutex);
#endif
}

void
run_task_sequentially(worker_t *worker) {
    while (true) {
        task_t *task = queue_front_pop(worker->task_queue);
        if (!task) return;
        step_task(worker, task);
    }
}

void
run_task(worker_t *worker) {
    if (single_threaded_flag) {
        run_task_sequentially(worker);
    } else {
        run_task_parallelly(worker);
    }
}
