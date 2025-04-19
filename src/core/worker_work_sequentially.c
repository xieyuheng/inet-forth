#include "index.h"

void
worker_work_sequentially(worker_t *worker) {
    while (true) {
        task_t *task = deque_pop_front(worker->task_deque);
        if (!task) return;
        worker_process_task(worker, task);
    }
}
