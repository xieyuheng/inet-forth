#include "index.h"

static atomic_int x, y;
static atomic_int a, b;

static void *
thread_fn_1(void *arg) {
    (void) arg;

    store_relaxed(&x, 1);
    store_relaxed(&a, load_relaxed(&y));

    return NULL;
}

static void *
thread_fn_2(void *arg) {
    (void) arg;

    store_relaxed(&y, 1);
    store_relaxed(&b, load_relaxed(&x));

    return NULL;
}

void
thread_test_weak_memory_dekker_relaxed(void) {
    printf("<thread_test_weak_memory_dekker_relaxed>\n");

    // comment the follow early `return` to run this test.
    // due to weak memory model, the program exit the follow loop,
    // and report after how many loops an unintuitive behavior occurred.

    return;

    size_t count = 0;

    do {
        store_relaxed(&x, 0);
        store_relaxed(&y, 0);

        thread_id_t thread_id_1 = thread_start(thread_fn_1, NULL);
        thread_id_t thread_id_2 = thread_start(thread_fn_2, NULL);

        thread_wait(thread_id_1);
        thread_wait(thread_id_2);

        count++;
    } while (load_relaxed(&a) != 0 || load_relaxed(&b) != 0);

    printf("count: %lu\n", count);

    printf("</thread_test_weak_memory_dekker_relaxed>\n");
}
