#include "index.h"

inline static task_t *
worker_steal_task(worker_t *worker) {
    scheduler_t *scheduler = worker->scheduler;
    size_t worker_count = scheduler_worker_count(scheduler);

    while (atomic_load_explicit(
               &scheduler->atomic_task_count,
               memory_order_acquire) > 0)
    {
        size_t victim_index = ++worker->victim_cursor % worker_count;
        if (victim_index == worker->index)
            victim_index = ++worker->victim_cursor % worker_count;

        worker_t *victim = array_get(scheduler->worker_array, victim_index);
        task_t *task = deque_pop_front(victim->task_deque);
        if (task) return task;
    }

    return NULL;
}

static void *
worker_thread_fn(void *arg) {
    worker_t *worker = arg;
    scheduler_t *scheduler = worker->scheduler;

    while (atomic_load_explicit(
               &scheduler->atomic_task_count,
               memory_order_acquire) > 0)
    {
        task_t *task = deque_pop_front(worker->task_deque);

#if DEBUG_WORK_STEALING_DISABLED
        (void) worker_steal_task;
#else
        if (!task) task = worker_steal_task(worker);
#endif
        if (!task) return NULL;

        worker_process_task(worker, task);
        atomic_fetch_sub_explicit(
            &scheduler->atomic_task_count,
            1,
            memory_order_release);
    }

    return NULL;
}

static void
scheduler_prepare(scheduler_t *scheduler, deque_t *init_task_deque) {
    size_t cursor = 0;
    while (!deque_is_empty(init_task_deque)) {
        task_t *task = deque_pop_front(init_task_deque);
        size_t index = cursor % scheduler_worker_count(scheduler);
        worker_t *worker = array_get(scheduler->worker_array, index);
        deque_push_back(worker->task_deque, task);
        atomic_fetch_add_explicit(
            &scheduler->atomic_task_count,
            1,
            memory_order_release);
        cursor++;
    }
}

void
worker_work_parallelly(worker_t *worker) {
    size_t processor_count = sysconf(_SC_NPROCESSORS_ONLN);
    size_t worker_count = processor_count - 1;
    scheduler_t *scheduler = scheduler_new(worker->mod, worker->node_allocator, worker_count);
    scheduler_prepare(scheduler, worker->task_deque);
    scheduler_start(scheduler, worker_thread_fn);
    scheduler_wait(scheduler);
    scheduler_destroy(&scheduler);
}
