#include "index.h"

static void
scheduler_start(scheduler_t *scheduler, queue_t *init_task_queue) {
    (void) scheduler;
    (void) init_task_queue;
}

static void
scheduler_wait(scheduler_t *scheduler) {
    (void) scheduler;
}

void
run_task_parallelly(worker_t *worker) {
    size_t processor_count = sysconf(_SC_NPROCESSORS_ONLN);
    size_t worker_count = processor_count - 1;
    scheduler_t *scheduler = scheduler_new(worker->mod, worker->node_allocator, worker_count);
    run_task_sequentially(worker);
    scheduler_start(scheduler, worker->task_queue);
    scheduler_wait(scheduler);
    scheduler_destroy(&scheduler);
}
