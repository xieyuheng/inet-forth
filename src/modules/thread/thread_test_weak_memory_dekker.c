#include "index.h"

static int x, y;
static int a, b;

static void *
thread_fn_1(void *arg) {
    (void) arg;

    x = 1;
    a = y;

    return NULL;
}

static void *
thread_fn_2(void *arg) {
    (void) arg;

    y = 1;
    b = x;

    return NULL;
}

void
thread_test_weak_memory_dekker(void) {
    printf("<thread_test_weak_memory_dekker>\n");

    // comment the follow early `return` to run this test.
    // due to weak memory model, the program exit the follow loop,
    // and report after how many loops an unintuitive behavior occurred.

    return;

    size_t count = 0;

    do {
        x = 0;
        y = 0;

        thread_id_t T1 = thread_start(thread_fn_1, NULL);
        thread_id_t T2 = thread_start(thread_fn_2, NULL);

        thread_wait(T1);
        thread_wait(T2);

        count++;
    } while (a != 0 || b != 0);

    printf("count: %lu\n", count);

    printf("</thread_test_weak_memory_dekker>\n");
}
