#include "index.h"

#define BIG 100000

static _Atomic uint64_t count = 0;

static void *
count_to_big(void *arg) {
    (void) arg;

    for (size_t i = 0; i < BIG; i++) {
        relaxed_store(&count, relaxed_load(&count) + 1);
        // count++;
    }

    return NULL;
}

void
test_thread_count_to_big(void) {
    test_start();

    tid_t T1 = thread_start(count_to_big, NULL);
    tid_t T2 = thread_start(count_to_big, NULL);

    thread_wait(T1);
    thread_wait(T2);

    who_printf("count: %lu\n", count);

    test_end();
}
