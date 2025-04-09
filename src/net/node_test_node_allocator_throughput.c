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
            // node_print(node, stdout);
            // printf("\n");
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

    thread_id_t T1 = thread_start(thread_fn, node_allocator);
    thread_id_t T2 = thread_start(thread_fn, node_allocator);
    thread_id_t T3 = thread_start(thread_fn, node_allocator);

    thread_wait(T1);
    thread_wait(T2);
    thread_wait(T3);

    node_allocator_destroy(&node_allocator);

    double throughput = REPEATION_COUNT * BATCH_SIZE / 1000 / time_passed_second(start_second);
    printf("throughput: %.f k/s\n", throughput);

    printf("</node_test_node_allocator_throughput>\n");
}
