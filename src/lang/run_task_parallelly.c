#include "index.h"

static bool
scheduler_no_more_task(scheduler_t *scheduler) {
    for (size_t i = 0; i < scheduler->worker_pool_size; i++) {
        if (!queue_is_empty(scheduler->task_queues[i])) {
            return false;
        }
    }

    return true;
}

static void
scheduler_dispatch_tasks(scheduler_t *scheduler) {
    size_t cursor = 0;
    for (size_t i = 0; i < scheduler->worker_pool_size; i++) {
        while (!queue_is_empty(scheduler->task_queues[i])) {
            task_t *task = queue_dequeue(scheduler->task_queues[i]);
            size_t real_cursor = cursor % scheduler->worker_pool_size;
            queue_enqueue(scheduler->workers[real_cursor]->task_queue, task);
            cursor++;
        }
    }
}

static void *
worker_thread_fn(worker_ctx_t *ctx) {
    worker_t *worker = ctx->worker;

    while (!queue_is_empty(worker->task_queue)) {
        step_task(worker);
    }

    return NULL;
}

static void
start_all_worker_threads(scheduler_t *scheduler) {
    for (size_t i = 0; i < scheduler->worker_pool_size; i++) {
        scheduler->worker_ctxs[i]->thread_id =
            thread_start((thread_fn_t *) worker_thread_fn, scheduler->worker_ctxs[i]);
    }
}

static void
wait_all_worker_threads(scheduler_t *scheduler) {
    for (size_t i = 0; i < scheduler->worker_pool_size; i++) {
        thread_wait(scheduler->worker_ctxs[i]->thread_id);
    }
}

static void
scheduler_cleanup(scheduler_t *scheduler) {
    for (size_t i = 0; i < scheduler->worker_pool_size; i++) {
        array_purge(scheduler->garbage_wire_arrays[i]);
    }
}

static void *
scheduler_thread_fn(scheduler_t *scheduler) {
    while (true) {
        if (scheduler_no_more_task(scheduler)) {
            return NULL;
        }

        scheduler_dispatch_tasks(scheduler);
        start_all_worker_threads(scheduler);
        wait_all_worker_threads(scheduler);
        scheduler_cleanup(scheduler);
    }
}

static void
scheduler_start(scheduler_t *scheduler, queue_t *init_task_queue) {
    // prepare tasks

    size_t cursor = 0;
    while (!queue_is_empty(init_task_queue)) {
        task_t *task = queue_dequeue(init_task_queue);
        size_t real_cursor = cursor % scheduler->worker_pool_size;
        queue_t *next_task_queue = scheduler->task_queues[real_cursor];
        bool ok = queue_enqueue(next_task_queue, task);
        assert(ok);
        cursor++;
    }

    // start scheduler thread

    scheduler->thread_id = thread_start((thread_fn_t *) scheduler_thread_fn, scheduler);
}

static void
scheduler_wait(scheduler_t *scheduler) {
    thread_wait(scheduler->thread_id);
}

void
run_task_parallelly(worker_t *worker) {
    size_t processor_count = sysconf(_SC_NPROCESSORS_ONLN);
    size_t worker_pool_size = uint_max(processor_count - 1, 1);
    scheduler_t *scheduler = scheduler_new(worker->mod, worker_pool_size);
    scheduler_start(scheduler, worker->task_queue);
    scheduler_wait(scheduler);
    scheduler_destroy(&scheduler);
}
