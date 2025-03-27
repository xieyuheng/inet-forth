#pragma once

void step_op(worker_t *worker, frame_t *frame, op_t *op);
void run_until(worker_t *worker, size_t base_length);

void step_net(worker_t *worker);
void run_net(worker_t *worker);
