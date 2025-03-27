#include "index.h"

value_t x_eq(value_t x, value_t y) {
    return xbool(x == y);
}

void x_dup(worker_t *worker) {
    value_t x = stack_pop(worker->value_stack);
    stack_push(worker->value_stack, x);
    stack_push(worker->value_stack, x);
}

void x_swap(worker_t *worker) {
    value_t y = stack_pop(worker->value_stack);
    value_t x = stack_pop(worker->value_stack);
    stack_push(worker->value_stack, y);
    stack_push(worker->value_stack, x);
}

void x_drop(worker_t *worker) {
    (void) stack_pop(worker->value_stack);
}

void x_rot(worker_t *worker) {
    value_t z = stack_pop(worker->value_stack);
    value_t y = stack_pop(worker->value_stack);
    value_t x = stack_pop(worker->value_stack);
    stack_push(worker->value_stack, y);
    stack_push(worker->value_stack, z);
    stack_push(worker->value_stack, x);
}

void x_over(worker_t *worker) {
    value_t y = stack_pop(worker->value_stack);
    value_t x = stack_pop(worker->value_stack);
    stack_push(worker->value_stack, x);
    stack_push(worker->value_stack, y);
    stack_push(worker->value_stack, x);
}

void x_tuck(worker_t *worker) {
    value_t y = stack_pop(worker->value_stack);
    value_t x = stack_pop(worker->value_stack);
    stack_push(worker->value_stack, y);
    stack_push(worker->value_stack, x);
    stack_push(worker->value_stack, y);
}
