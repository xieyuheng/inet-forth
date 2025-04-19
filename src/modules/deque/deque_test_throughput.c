#include "index.h"

#define LENGTH 10000000

static void *
uint_producer(deque_t *deque) {
    size_t count = 0;
    while (true) {
        if (count == LENGTH) return NULL;

        deque_push_back(deque, (void *) count);
        count++;
    }
}

static void *
uint_consumer(deque_t *deque) {
    size_t count = 0;
    while (true) {
        if (count == LENGTH) return NULL;

        while (deque_is_empty(deque)) {}

        assert(((size_t) deque_pop_front(deque)) == count);
        count++;
    }
}

void
deque_test_throughput(void) {
    test_start();

    deque_t *deque = deque_new();

    double start_second = time_second();

    tid_t producer_id =
        thread_start((thread_fn_t *) uint_producer, deque);
    tid_t consumer_id =
        thread_start((thread_fn_t *) uint_consumer, deque);

    thread_wait(producer_id);
    thread_wait(consumer_id);

    double throughput = LENGTH / 1000 / time_passed_second(start_second);
    test_printf("throughput: %.f k/s\n", throughput);

    deque_destroy(&deque);

    test_end();
}
