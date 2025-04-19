#pragma once

void run_until(worker_t *worker, size_t base_length);

void step_task(worker_t *worker, task_t *task);
void run_task_sequentially(worker_t *worker);
void run_task_parallelly(worker_t *worker);
void run_task(worker_t *worker);
