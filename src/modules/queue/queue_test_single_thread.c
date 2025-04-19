#include "index.h"

void
queue_test_single_thread(void) {
    test_start();

    queue_t *queue = queue_new(4);
    assert(queue_size(queue) == 4);
    assert(queue_length(queue) == 0);

    assert(queue_push_back(queue, (void *) 1));
    assert(queue_push_back(queue, (void *) 2));
    assert(queue_push_back(queue, (void *) 3));
    assert(queue_push_back(queue, (void *) 4));

    assert(((uint64_t) queue_get(queue, 0)) == 1);
    assert(((uint64_t) queue_get(queue, 1)) == 2);
    assert(((uint64_t) queue_get(queue, 2)) == 3);
    assert(((uint64_t) queue_get(queue, 3)) == 4);

    assert(queue_length(queue) == 4);
    assert(queue_is_full(queue));

    assert(((uint64_t) queue_pop_front(queue)) == 1);
    assert(((uint64_t) queue_pop_front(queue)) == 2);
    assert(((uint64_t) queue_pop_front(queue)) == 3);
    assert(((uint64_t) queue_pop_front(queue)) == 4);
    assert(queue_pop_front(queue) == NULL);

    assert(queue_length(queue) == 0);

    queue_destroy(&queue);

    test_end();
}
