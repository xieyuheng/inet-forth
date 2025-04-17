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

static atomic_size_t global_count = 0;
static atomic_bool eventual_run_p = false;
static atomic_bool eventual_stop_p = false;

static void *
eventual(void *arg) {
    (void) arg;

    while (true) {
        if (relaxed_load(&eventual_stop_p)) {
            return NULL;
        }

        if (relaxed_load(&eventual_run_p)) {
            size_t total_count = 0;
            for (size_t i = 0; i < THREAD_NUMBER; i++) {
                total_count += relaxed_load(&counts[i]);
            }

            relaxed_store(&global_count, total_count);
        }
    }
}

static size_t
counter_read(void) {
    return relaxed_load(&global_count);
}

void
thread_test_counter_stat_eventual(void) {
    test_start();
    double start_second = time_second();

    list_t *list = list_new();

    tid_t eventual_tid = thread_start(eventual, NULL);
    relaxed_store(&eventual_run_p, true);

    thread_fn_t *thread_fn = (thread_fn_t *) counter_add1;
    for (size_t i = 0; i < THREAD_NUMBER; i++) {
        tid_t tid = thread_start(thread_fn, &counts[i]);
        list_push(list, (void *) tid);
    }

    test_printf("final count: %lu\n", counter_read());
    relaxed_store(&eventual_run_p, false);

    while (!list_is_empty(list)) {
        tid_t tid = (tid_t) list_pop(list);
        thread_wait(tid);
    }

    relaxed_store(&eventual_run_p, true);
    sleep(0);
    test_printf("final count: %lu\n", counter_read());
    relaxed_store(&eventual_run_p, false);

    relaxed_store(&eventual_stop_p, true);
    thread_wait(eventual_tid);

    list_destroy(&list);

    test_printf("elapsed seconds: %fs\n", time_passed_second(start_second));
    test_end();
}
