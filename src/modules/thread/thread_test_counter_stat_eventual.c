#include "index.h"

static void *
counter_add1(atomic_size_t *count_pointer) {
    size_t count = load_relaxed(count_pointer);
    sleep(0); // let other threads run
    store_relaxed(count_pointer, count + 1);
    return NULL;
}

#define THREAD_NUMBER 1000

static atomic_size_t counts[THREAD_NUMBER];

static atomic_size_t global_count = 0;
static atomic_bool eventual_run_p = false;
static atomic_bool eventual_stop_p = false;

static void *
eventual(void *arg) {
    (void) arg;

    while (true) {
        if (load_relaxed(&eventual_stop_p)) {
            return NULL;
        }

        if (load_relaxed(&eventual_run_p)) {
            size_t total_count = 0;
            for (size_t i = 0; i < THREAD_NUMBER; i++) {
                total_count += load_relaxed(&counts[i]);
            }

            store_relaxed(&global_count, total_count);
        }
    }
}

static size_t
counter_read(void) {
    return load_relaxed(&global_count);
}

void
thread_test_counter_stat_eventual(void) {
    printf("<thread_test_counter_stat_eventual>\n");
    clock_t start_clock = clock();

    list_t *list = list_new();

    thread_id_t eventual_thread_id = thread_start(eventual, NULL);
    store_relaxed(&eventual_run_p, true);

    thread_fn_t *thread_fn = (thread_fn_t *) counter_add1;
    for (size_t i = 0; i < THREAD_NUMBER; i++) {
        thread_id_t thread_id = thread_start(thread_fn, &counts[i]);
        list_push(list, (void *) thread_id);
    }

    printf("final count: %lu\n", counter_read());
    store_relaxed(&eventual_run_p, false);

    while (!list_is_empty(list)) {
        thread_id_t thread_id = (thread_id_t) list_pop(list);
        thread_wait(thread_id);
    }

    store_relaxed(&eventual_run_p, true);
    sleep(0);
    printf("final count: %lu\n", counter_read());
    store_relaxed(&eventual_run_p, false);

    store_relaxed(&eventual_stop_p, true);
    thread_wait(eventual_thread_id);

    list_destroy(&list);

    printf("elapsed seconds: %fs\n", clock_elapsed_seconds(start_clock));
    printf("</thread_test_counter_stat_eventual>\n");
}
