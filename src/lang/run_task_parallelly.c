#include "index.h"

static task_t *
worker_steal_task(worker_t *worker) {
    scheduler_t *scheduler = worker->scheduler;
    size_t worker_count = scheduler_worker_count(scheduler);

    while (atomic_load(&scheduler->atomic_task_count) > 0) {
        size_t victim_index = ++worker->victim_cursor % worker_count;
        if (victim_index == worker->index)
            victim_index = ++worker->victim_cursor % worker_count;

        worker_t *victim = array_get(scheduler->worker_array, victim_index);
        mutex_lock(victim->task_queue_front_mutex);
        task_t *task = queue_front_pop(victim->task_queue);
        mutex_unlock(victim->task_queue_front_mutex);

        if (task) return task;
    }

    return NULL;
}


static void *
worker_thread_fn(void *arg) {
    worker_t *worker = arg;
    scheduler_t *scheduler = worker->scheduler;

    while (atomic_load(&scheduler->atomic_task_count) > 0) {
        mutex_lock(worker->task_queue_front_mutex);
        task_t *task = queue_front_pop(worker->task_queue);
        mutex_unlock(worker->task_queue_front_mutex);

        if (!task) task = worker_steal_task(worker);
        if (!task) return NULL;;

        step_task(worker, task);
        atomic_fetch_sub(&scheduler->atomic_task_count, 1);
    }

    return NULL;
}

static void
scheduler_prepare(scheduler_t *scheduler, queue_t *init_task_queue) {
    size_t cursor = 0;
    while (!queue_is_empty(init_task_queue)) {
        atomic_fetch_add(&scheduler->atomic_task_count, 1);
        task_t *task = queue_front_pop(init_task_queue);
        size_t index = cursor % scheduler_worker_count(scheduler);
        worker_t *worker = array_get(scheduler->worker_array, index);
        bool ok = queue_back_push(worker->task_queue, task);
        assert(ok);
        cursor++;
    }
}

static void
scheduler_start(scheduler_t *scheduler) {
    for (size_t i = 0; i < array_length(scheduler->worker_array); i++) {
        worker_t *worker = array_get(scheduler->worker_array, i);
        tid_t tid = thread_start(worker_thread_fn, worker);
        array_set(scheduler->worker_tid_array, i, (void *) (uint64_t) tid);
    }
}

static void
scheduler_wait(scheduler_t *scheduler) {
    for (size_t i = 0; i < array_length(scheduler->worker_tid_array); i++) {
        tid_t tid = (tid_t) array_get(scheduler->worker_tid_array, i);
        thread_wait(tid);
    }
}

void
run_task_parallelly(worker_t *worker) {
    size_t processor_count = sysconf(_SC_NPROCESSORS_ONLN);
    size_t worker_count = processor_count - 1;
    scheduler_t *scheduler = scheduler_new(worker->mod, worker->node_allocator, worker_count);
    scheduler_prepare(scheduler, worker->task_queue);
    scheduler_start(scheduler);
    scheduler_wait(scheduler);
    scheduler_destroy(&scheduler);
}
