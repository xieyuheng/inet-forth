#include "index.h"

void
execute_operation(vm_t *vm, frame_t *frame, op_t *unknown_op) {
    switch (unknown_op->kind) {
    case OP_CALL: {
        call_op_t *op = (call_op_t *) unknown_op;
        call(vm, op->def);
        return;
    }

    case OP_LITERAL: {
        literal_op_t *op = (literal_op_t *) unknown_op;
        stack_push(vm->value_stack, op->value);
        return;
    }

    case OP_LOCAL_GET: {
        local_get_op_t *op = (local_get_op_t *) unknown_op;
        value_t value = frame_local_get(frame, op->index);
        stack_push(vm->value_stack, value);
        return;
    }

    case OP_LOCAL_SET: {
        local_set_op_t *op = (local_set_op_t *) unknown_op;
        value_t value = stack_pop(vm->value_stack);
        frame_local_set(frame, op->index, value);
        return;
    }
    }
}
