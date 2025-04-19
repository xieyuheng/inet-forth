#pragma once

void run_until(worker_t *worker, size_t base_length);

void worker_process_all_tasks_sequentially(worker_t *worker);
void worker_process_all_tasks_parallelly(worker_t *worker);
void worker_process_all_tasks(worker_t *worker);
