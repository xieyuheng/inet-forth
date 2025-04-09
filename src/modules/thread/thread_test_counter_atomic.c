#include "index.h"

static atomic_size_t global_count = 0;

static void *
counter_add1(void *arg) {
    (void) arg;
    atomic_fetch_add(&global_count, 1);
    sleep(0); // let other threads run
    return NULL;
}

static size_t
counter_read(void) {
    return atomic_load(&global_count);
}

void
thread_test_counter_atomic(void) {
    printf("<thread_test_counter_atomic>\n");
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

    printf("final count: %lu\n", counter_read());
    printf("elapsed seconds: %fs\n", time_passed_second(start_second));
    printf("</thread_test_counter_atomic>\n");
}
