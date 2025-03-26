#include "index.h"

#define QUEUE_SIZE 1024
#define LENGTH 10000000

static void *
uint_producer(queue_t *queue) {
    size_t count = 0;
    while (true) {
        if (count == LENGTH) return NULL;

        while (queue_is_full(queue)) {}

        assert(queue_enqueue(queue, (void *) count));
        count++;
    }
}

static void *
uint_consumer(queue_t *queue) {
    size_t count = 0;
    while (true) {
        if (count == LENGTH) return NULL;

        while (queue_is_empty(queue)) {}

        assert(((size_t) queue_dequeue(queue)) == count);
        count++;
    }
}

void
queue_test_throughput(void) {
    printf("<queue_test_throughput>\n");

    queue_t *queue = queue_new(QUEUE_SIZE);

    printf("uint_producer v.s. uint_consumer\n");

    double start_second = time_second();

    thread_id_t producer_id =
        thread_start((thread_fn_t *) uint_producer, queue);
    thread_id_t consumer_id =
        thread_start((thread_fn_t *) uint_consumer, queue);

    thread_wait(producer_id);
    thread_wait(consumer_id);

    double end_second = time_second();
    double passed_second = end_second - start_second;
    printf("throughput: %.f k/s\n", LENGTH / passed_second / 1000);

    queue_destroy(&queue);

    printf("</queue_test_throughput>\n");
}
