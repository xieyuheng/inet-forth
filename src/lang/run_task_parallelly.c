#include "index.h"

static void *
worker_thread_fn(void *arg) {
    (void) arg;
    return NULL;
}

static void
scheduler_start(scheduler_t *scheduler, queue_t *init_task_queue) {
    (void) init_task_queue;
    for (size_t i = 0; i < array_length(scheduler->worker_array); i++) {
        worker_t *worker = array_get(scheduler->worker_array, i);
        tid_t tid = thread_start(worker_thread_fn, worker);
        array_set(scheduler->worker_tid_array, i, (void *) (uint64_t) tid);
    }
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
