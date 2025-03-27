#pragma once

void run_until(worker_t *worker, size_t base_length);

void step_net(worker_t *worker);
void run_net(worker_t *worker);
