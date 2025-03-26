#include "index.h"

void
queue_test_single_thread(void) {
    printf("<queue_test_single_thread>\n");

    queue_t *queue = queue_new(4);
    assert(queue_size(queue) == 4);
    assert(queue_length(queue) == 0);

    assert(queue_enqueue(queue, (void *) 1));
    assert(queue_enqueue(queue, (void *) 2));
    assert(queue_enqueue(queue, (void *) 3));
    assert(queue_enqueue(queue, (void *) 4));

    assert(((uint64_t) queue_get(queue, 0)) == 1);
    assert(((uint64_t) queue_get(queue, 1)) == 2);
    assert(((uint64_t) queue_get(queue, 2)) == 3);
    assert(((uint64_t) queue_get(queue, 3)) == 4);

    assert(queue_length(queue) == 4);
    assert(queue_is_full(queue));

    assert(((uint64_t) queue_dequeue(queue)) == 1);
    assert(((uint64_t) queue_dequeue(queue)) == 2);
    assert(((uint64_t) queue_dequeue(queue)) == 3);
    assert(((uint64_t) queue_dequeue(queue)) == 4);
    assert(queue_dequeue(queue) == NULL);

    assert(queue_length(queue) == 0);

    queue_destroy(&queue);

    printf("</queue_test_single_thread>\n");
}
