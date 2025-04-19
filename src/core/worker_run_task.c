#include "index.h"

void
worker_run_task(worker_t *worker, task_t *task) {
#if DEBUG_TASK_LOCK
    mutex_t *mutex = task->mutex;
    while (!mutex_try_lock(mutex)) {
        file_lock(stdout);
        who_printf("lock contention! ");
        printf("worker #%ld, ", worker->index);
        printf("task: "); task_print(task, stdout);
        printf("\n");
        file_unlock(stdout);
    }
#endif

#if DEBUG_TASK_LOG
    file_lock(stdout);
    who_printf("worker #%ld, ", worker->index);
    printf("task: "); task_print(task, stdout);
    printf("\n");
    file_unlock(stdout);
#endif
    // NOTE The follow assertions can be used to verify that
    // the oppsite node in a task can not be synchronized by the task queue.
    // Thus lock on node (in `worker_disconnect_node`) is required.

    // {
    //     assert(acquire_load(&task->left->node->atomic_is_ready));
    //     assert(acquire_load(&task->right->node->atomic_is_ready));
    // }

    worker_disconnect_node(worker, task->left->node);
    worker_disconnect_node(worker, task->right->node);

    size_t base_length = stack_length(worker->return_stack);
    frame_t *frame = frame_new(task->rule->function);
    task_destroy(&task);
    stack_push(worker->return_stack, frame);
    run_until(worker, base_length);

#if DEBUG_TASK_LOCK
    mutex_unlock(mutex);
    mutex_destroy(&mutex);
#endif
}
