#include "index.h"

static atomic_size_t global_count = 0;

static void *
counter_add1(void *arg) {
    (void) arg;
    size_t count = relaxed_load(&global_count);
    sleep(0); // let other threads run
    relaxed_store(&global_count, count + 1);
    return NULL;
}

static size_t
counter_read(void) {
    return relaxed_load(&global_count);
}

void
test_thread_counter_non_atomic(void) {
    test_start();
    double start_second = time_second();

    list_t *list = list_new();

    thread_fn_t *thread_fn = (thread_fn_t *) counter_add1;
    for (size_t i = 0; i < 1000; i++) {
        tid_t tid = thread_start(thread_fn, NULL);
        list_push(list, (void *) tid);
    }

    while (!list_is_empty(list)) {
        tid_t tid = (tid_t) list_pop(list);
        thread_wait(tid);
    }

    list_destroy(&list);

    who_printf("final count: %lu\n", counter_read());
    who_printf("elapsed seconds: %fs\n", time_passed_second(start_second));
    test_end();
}
