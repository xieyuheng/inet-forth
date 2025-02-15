#include "index.h"

void
step_op(vm_t *vm, frame_t *frame, op_t *op) {
    switch (op->kind) {
    case OP_CALL: {
        call(vm, op->call.def);
        return;
    }

    case OP_LITERAL: {
        stack_push(vm->value_stack, op->literal.value);
        return;
    }

    case OP_GET_VARIABLE: {
        value_t value = frame_get_variable(frame, op->get_variable.index);
        stack_push(vm->value_stack, value);
        return;
    }

    case OP_SET_VARIABLE: {
        value_t value = stack_pop(vm->value_stack);
        frame_set_variable(frame, op->set_variable.index, value);
        return;
    }
    }
}

void
step(vm_t *vm) {
    if (stack_is_empty(vm->return_stack)) return;

    frame_t *frame = stack_pop(vm->return_stack);
    if (frame_is_finished(frame)) {
        frame_destroy(&frame);
        return;
    }

    op_t *op = frame_fetch_op(frame);

    // proper tail-call = do not push finished frame.
    bool finished = frame_is_finished(frame);
    if (!finished) {
        stack_push(vm->return_stack, frame);
    }

    step_op(vm, frame, op);

    if (finished) {
        frame_destroy(&frame);
    }
}

void
run_until(vm_t *vm, size_t base_length) {
    if (vm->log_level > 0) {
        vm_print(vm, stdout);
        fprintf(stdout, "\n");
    }

    while (stack_length(vm->return_stack) > base_length) {
        step(vm);

        if (vm->log_level > 0) {
            vm_print(vm, stdout);
            fprintf(stdout, "\n");
        }
    }
}

static void
collect_free_wires_from_node(vm_t *vm, node_t *node) {
    for (size_t i = 0; i < node->ctor->arity; i++) {
        if (!wire_is_principal(node->wires[i])) {
            wire_t *wire = node->wires[i];
            wire_free_from_node(wire);
            stack_push(vm->value_stack, wire);
        }
    }

    vm_delete_node(vm, node);
}

void
step_net(vm_t *vm) {
    activity_t *activity = list_shift(vm->activity_list);
    if (!activity) return;

    node_t *first_node = activity->wire->node;
    node_t *second_node = activity->wire->opposite->node;

    if (first_node->ctor == activity->rule->second_node_ctor &&
        second_node->ctor == activity->rule->first_node_ctor)
    {
        first_node = activity->wire->opposite->node;
        second_node = activity->wire->node;
    }

    collect_free_wires_from_node(vm, first_node);
    collect_free_wires_from_node(vm, second_node);

    vm_delete_wire(vm, activity->wire);
    vm_delete_wire(vm, activity->wire->opposite);

    size_t base_length = stack_length(vm->return_stack);
    frame_t *frame = frame_new(activity->rule->function);

    activity_destroy(&activity);

    stack_push(vm->return_stack, frame);
    run_until(vm, base_length);
}

void
run_net(vm_t *vm) {
    while (!list_is_empty(vm->activity_list)) {
        step_net(vm);
    }
}
