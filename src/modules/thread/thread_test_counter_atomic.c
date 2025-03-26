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
    clock_t start_clock = clock();

    list_t *list = list_new();

    thread_fn_t *thread_fn = (thread_fn_t *) counter_add1;
    for (size_t i = 0; i < 1000; i++) {
        thread_id_t thread_id = thread_start(thread_fn, NULL);
        list_push(list, (void *) thread_id);
    }

    while (!list_is_empty(list)) {
        thread_id_t thread_id = (thread_id_t) list_pop(list);
        thread_wait(thread_id);
    }

    list_destroy(&list);

    printf("final count: %lu\n", counter_read());
    printf("elapsed seconds: %fs\n", clock_elapsed_seconds(start_clock));
    printf("</thread_test_counter_atomic>\n");
}
