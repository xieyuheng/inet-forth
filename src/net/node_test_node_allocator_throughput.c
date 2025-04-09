#include "index.h"

void
node_test_node_allocator_throughput(void) {
    printf("<node_test_node_allocator_throughput>\n");

    node_allocator_t *node_allocator = node_allocator_new();

    double start_second = time_second();

    stack_t *stack_1 = stack_new();
    stack_t *stack_2 = stack_new();
    node_allocator_add_per_thread_stack(node_allocator, stack_1);
    node_allocator_add_per_thread_stack(node_allocator, stack_2);
    assert(node_allocator_thread_count(node_allocator) == 2);

    {
        node_t *node = node_allocator_allocate(node_allocator, stack_1);
        node_print(node, stdout);
        printf("\n");
        node_allocator_recycle(node_allocator, stack_1, &node);
    }

    {
        node_t *node = node_allocator_allocate(node_allocator, stack_1);
        node_print(node, stdout);
        printf("\n");
        node_allocator_recycle(node_allocator, stack_1, &node);
    }

    {
        node_t *node = node_allocator_allocate(node_allocator, stack_1);
        node_print(node, stdout);
        printf("\n");
        node_allocator_recycle(node_allocator, stack_1, &node);
    }

    node_allocator_destroy(&node_allocator);

    double end_second = time_second();
    double passed_second = end_second - start_second;
    printf("overhead of %d nodes: %.f ms\n",
           NODE_ALLOCATOR_BATCH_SIZE,
           passed_second * 1000);

    printf("</node_test_node_allocator_throughput>\n");
}
