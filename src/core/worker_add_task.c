#include "index.h"

void
worker_add_task(worker_t *worker, task_t *task) {
    deque_push_back(worker->task_deque, task);

    if (worker->scheduler) {
        atomic_fetch_add_explicit(
            &worker->scheduler->atomic_task_count,
            1,
            memory_order_release);
    }
}
