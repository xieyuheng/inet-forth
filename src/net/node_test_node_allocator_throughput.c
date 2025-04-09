#include "index.h"

#define BATCH_SIZE 1000
#define REPEATION_COUNT 10000

static void *
thread_fn(void *arg) {
    node_allocator_t *node_allocator = arg;
    stack_t *stack = stack_new();
    node_allocator_add_per_thread_stack(node_allocator, stack);

    stack_t *allocated_stack = stack_new();
    for (size_t r = 0; r < REPEATION_COUNT; r++) {
        for (size_t i = 0; i < BATCH_SIZE; i++) {
            node_t *node = node_allocator_allocate(node_allocator, stack);
            stack_push(allocated_stack, node);
        }

        for (size_t i = 0; i < BATCH_SIZE; i++) {
            node_t *node = stack_pop(allocated_stack);
            node_allocator_recycle(node_allocator, stack, &node);
        }
    }

    return NULL;
}

void
node_test_node_allocator_throughput(void) {
    printf("<node_test_node_allocator_throughput>\n");

    node_allocator_t *node_allocator = node_allocator_new();

    double start_second = time_second();

    size_t thread_count = 3;
    array_t *thread_array = array_auto();
    for (size_t i = 0; i < thread_count; i++) {
        tid_t tid = thread_start(thread_fn, node_allocator);
        array_push(thread_array, (void *) tid);
    }

    for (size_t i = 0; i < thread_count; i++) {
        tid_t tid = (tid_t) array_pop(thread_array);
        thread_wait(tid);
    }

    node_allocator_destroy(&node_allocator);

    printf("thread_count: %lu\n", thread_count);
    double throughput = REPEATION_COUNT * BATCH_SIZE / 1000 / time_passed_second(start_second);
    printf("throughput: %.f k/s\n", throughput);

    printf("</node_test_node_allocator_throughput>\n");
}
