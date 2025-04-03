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
}

void
run_until(worker_t *worker, size_t base_length) {
    while (stack_length(worker->return_stack) > base_length) {
        step(worker);
    }
}

static void
collect_free_wires_from_node(worker_t *worker, node_t *node) {
    for (size_t i = 0; i < node->ctor->arity; i++) {
        value_t value = node_get(node, i);
        if (is_wire(value)) {
            wire_t *wire = as_wire(value);
            if (wire_is_principal(value)) {
                continue;
            } else {
                wire_free_from_node(wire);
                stack_push(worker->value_stack, wire);
            }
        } else {
            stack_push(worker->value_stack, value);
        }
    }

    worker_delete_node(worker, node);
}

void
step_task(worker_t *worker) {
    task_t *task = queue_dequeue(worker->task_queue);
    if (!task) return;

    wire_t *opposite = wire_opposite(task->wire);

    node_t *first_node = task->wire->node;
    node_t *second_node = opposite->node;

    if (first_node->ctor == task->rule->second_node_ctor &&
        second_node->ctor == task->rule->first_node_ctor)
    {
        first_node = opposite->node;
        second_node = task->wire->node;
    }

    collect_free_wires_from_node(worker, first_node);
    collect_free_wires_from_node(worker, second_node);

    worker_delete_wire(worker, opposite);
    worker_delete_wire(worker, task->wire);

    size_t base_length = stack_length(worker->return_stack);
    frame_t *frame = frame_new(task->rule->function);

    task_destroy(&task);

    stack_push(worker->return_stack, frame);
    run_until(worker, base_length);
}

void
run_task(worker_t *worker) {
    while (!queue_is_empty(worker->task_queue)) {
        step_task(worker);
    }
}
