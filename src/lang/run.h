#pragma once

void step_op(vm_t *vm, frame_t *frame, op_t *op);
void step(vm_t *vm);
void run_until(vm_t *vm, size_t base_length);

void step_net(vm_t *vm);
void run_net(vm_t *vm);
