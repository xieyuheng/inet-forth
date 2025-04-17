#include "index.h"

void
deque_test(void) {
    test_start();

    deque_t *deque = deque_new();

    assert(deque_is_empty(deque));
    assert(deque_length(deque) == 0);

    deque_destroy(&deque);

    test_end();
}
