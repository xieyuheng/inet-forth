#include "index.h"

static void *
counter_add1(atomic_size_t *count_pointer) {
    size_t count = relaxed_load(count_pointer);
    sleep(0); // let other threads run
    relaxed_store(count_pointer, count + 1);
    return NULL;
}

#define THREAD_NUMBER 1000

static atomic_size_t counts[THREAD_NUMBER];

static size_t
counter_read(void) {
    size_t total_count = 0;
    for (size_t i = 0; i < THREAD_NUMBER; i++) {
        total_count += relaxed_load(&counts[i]);
    }

    return total_count;
}

void
thread_test_counter_stat(void) {
    printf("<thread_test_counter_stat>\n");
    double start_second = time_second();

    list_t *list = list_new();

    thread_fn_t *thread_fn = (thread_fn_t *) counter_add1;
    for (size_t i = 0; i < THREAD_NUMBER; i++) {
        tid_t tid = thread_start(thread_fn, &counts[i]);
        list_push(list, (void *) tid);
    }

    while (!list_is_empty(list)) {
        tid_t tid = (tid_t) list_pop(list);
        thread_wait(tid);
    }

    list_destroy(&list);

    printf("final count: %lu\n", counter_read());
    printf("elapsed seconds: %fs\n", time_passed_second(start_second));
    printf("</thread_test_counter_stat>\n");
}
