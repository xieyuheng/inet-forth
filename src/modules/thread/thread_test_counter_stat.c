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
    clock_t start_clock = clock();

    list_t *list = list_new();

    thread_fn_t *thread_fn = (thread_fn_t *) counter_add1;
    for (size_t i = 0; i < THREAD_NUMBER; i++) {
        thread_id_t thread_id = thread_start(thread_fn, &counts[i]);
        list_push(list, (void *) thread_id);
    }

    while (!list_is_empty(list)) {
        thread_id_t thread_id = (thread_id_t) list_pop(list);
        thread_wait(thread_id);
    }

    list_destroy(&list);

    printf("final count: %lu\n", counter_read());
    printf("elapsed seconds: %fs\n", clock_elapsed_seconds(start_clock));
    printf("</thread_test_counter_stat>\n");
}
