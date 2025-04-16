#include "index.h"

void
worker_add_task(worker_t *self, task_t *task) {
    queue_back_push(self->task_queue, task);
    if (self->scheduler) {
        atomic_add1(&self->scheduler->atomic_task_count);
    }
}
