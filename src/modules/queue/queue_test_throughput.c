#include "index.h"

#define QUEUE_SIZE 1024
#define LENGTH 10000000

static void *
uint_producer(queue_t *queue) {
    size_t count = 0;
    while (true) {
        if (count == LENGTH) return NULL;

        while (queue_is_full(queue)) {}

        assert(queue_back_push(queue, (void *) count));
        count++;
    }
}

static void *
uint_consumer(queue_t *queue) {
    size_t count = 0;
    while (true) {
        if (count == LENGTH) return NULL;

        while (queue_is_empty(queue)) {}

        assert(((size_t) queue_front_pop(queue)) == count);
        count++;
    }
}

void
queue_test_throughput(void) {
    test_start();

    queue_t *queue = queue_new(QUEUE_SIZE);

    printf("uint_producer v.s. uint_consumer\n");

    double start_second = time_second();

    tid_t producer_id =
        thread_start((thread_fn_t *) uint_producer, queue);
    tid_t consumer_id =
        thread_start((thread_fn_t *) uint_consumer, queue);

    thread_wait(producer_id);
    thread_wait(consumer_id);

    double throughput = LENGTH / 1000 / time_passed_second(start_second);
    printf("throughput: %.f k/s\n", throughput);

    queue_destroy(&queue);

    test_end();
}
