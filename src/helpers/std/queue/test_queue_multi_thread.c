#include "index.h"

#define QUEUE_SIZE 4
#define LENGTH 1000000

static void *
uint_producer(queue_t *queue) {
    size_t count = 0;
    while (true) {
        if (count == LENGTH) return NULL;

        while (queue_is_full(queue)) {}

        assert(queue_push_back(queue, (void *) count));
        count++;
    }
}

static void *
uint_consumer(queue_t *queue) {
    size_t count = 0;
    while (true) {
        if (count == LENGTH) return NULL;

        while (queue_is_empty(queue)) {}

        assert(((size_t) queue_pop_front(queue)) == count);
        count++;
    }
}

static void *
string_producer(queue_t *queue) {
    size_t count = 0;
    while (true) {
        if (count == LENGTH) return NULL;

        while (queue_is_full(queue)) {}

        assert(queue_push_back(queue, uint_to_string(count)));
        count++;
    }
}

static void *
string_consumer(queue_t *queue) {
    size_t count = 0;
    while (true) {
        if (count == LENGTH) return NULL;

        while (queue_is_empty(queue)) {}

        char *x = queue_pop_front(queue);
        char *y = uint_to_string(count);
        assert(string_equal(x, y));
        string_destroy(&x);
        string_destroy(&y);
        count++;
    }
}

void
test_queue_multi_thread(void) {
    test_start();

    queue_t *queue = queue_new(QUEUE_SIZE);

    {
        who_printf("uint_producer vs. uint_consumer\n");

        tid_t producer_id =
            thread_start((thread_fn_t *) uint_producer, queue);
        tid_t consumer_id =
            thread_start((thread_fn_t *) uint_consumer, queue);

        thread_wait(producer_id);
        thread_wait(consumer_id);
    }

    {
        who_printf("string_producer vs. string_consumer\n");

        tid_t producer_id =
            thread_start((thread_fn_t *) string_producer, queue);
        tid_t consumer_id =
            thread_start((thread_fn_t *) string_consumer, queue);

        thread_wait(producer_id);
        thread_wait(consumer_id);
    }

    queue_destroy(&queue);

    test_end();
}
