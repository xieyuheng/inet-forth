#include "index.h"

static atomic_int x, y;
static atomic_int a, b;

static void *
thread_fn_1(void *arg) {
    (void) arg;

    relaxed_store(&x, 1);
    relaxed_store(&a, relaxed_load(&y));

    return NULL;
}

static void *
thread_fn_2(void *arg) {
    (void) arg;

    relaxed_store(&y, 1);
    relaxed_store(&b, relaxed_load(&x));

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
        relaxed_store(&x, 0);
        relaxed_store(&y, 0);

        thread_id_t thread_id_1 = thread_start(thread_fn_1, NULL);
        thread_id_t thread_id_2 = thread_start(thread_fn_2, NULL);

        thread_wait(thread_id_1);
        thread_wait(thread_id_2);

        count++;
    } while (relaxed_load(&a) != 0 || relaxed_load(&b) != 0);

    printf("count: %lu\n", count);

    printf("</thread_test_weak_memory_dekker_relaxed>\n");
}
