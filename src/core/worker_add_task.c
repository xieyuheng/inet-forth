#include "index.h"

void
worker_add_task(worker_t *worker, task_t *task) {
    queue_back_push(worker->task_queue, task);
    if (worker->scheduler) {
        atomic_add1(&worker->scheduler->atomic_task_count);
    }
}
