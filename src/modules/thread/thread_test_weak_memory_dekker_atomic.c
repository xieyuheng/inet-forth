#include "index.h"

static atomic_int x, y;
static atomic_int a, b;

static void *
thread_fn_1(void *arg) {
    (void) arg;

    atomic_store(&x, 1);
    atomic_store(&a, atomic_load(&y));

    return NULL;
}

static void *
thread_fn_2(void *arg) {
    (void) arg;

    atomic_store(&y, 1);
    atomic_store(&b, atomic_load(&x));

    return NULL;
}

void
thread_test_weak_memory_dekker_atomic(void) {
    printf("<thread_test_weak_memory_dekker_atomic>\n");

    // comment the follow early `return` to run this test.
    // unlike the relaxed version, this program should loop forever.

    return;

    size_t count = 0;

    do {
        atomic_store(&x, 0);
        atomic_store(&y, 0);

        thread_id_t thread_id_1 = thread_start(thread_fn_1, NULL);
        thread_id_t thread_id_2 = thread_start(thread_fn_2, NULL);

        thread_wait(thread_id_1);
        thread_wait(thread_id_2);

        count++;
    } while (atomic_load(&a) != 0 || atomic_load(&b) != 0);

    printf("count: %lu\n", count);

    printf("</thread_test_weak_memory_dekker_atomic>\n");
}
