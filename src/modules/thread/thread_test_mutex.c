#include "index.h"

static size_t loop_count = 10000;
static uint64_t global_count = 0;

static void *
thread_fn(void *arg) {
    mutex_t *mutex = arg;
    size_t count = 0;
    while (count < loop_count) {
        {
            mutex_lock(mutex);

            global_count++;
            sleep(0);

            mutex_unlock(mutex);
        }

        sleep(0);
        count++;
    }

    return NULL;
}

void
thread_test_mutex(void) {
    printf("<thread_test_mutex>\n");

    mutex_t *mutex = mutex_new();

    thread_id_t thread_id_1 = thread_start(thread_fn, mutex);
    thread_id_t thread_id_2 = thread_start(thread_fn, mutex);

    thread_wait(thread_id_1);
    thread_wait(thread_id_2);

    assert(global_count == loop_count * 2);

    printf("</thread_test_mutex>\n");
}
