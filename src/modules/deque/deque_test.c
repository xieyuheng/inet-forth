#include "index.h"

void
deque_test(void) {
    test_start();

    deque_t *deque = deque_new();
    deque_destroy(&deque);

    test_end();
}
