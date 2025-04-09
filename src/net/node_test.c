#include "index.h"

void
node_test(void) {
    printf("<node_test>\n");

    double start_second = time_second();

    node_allocator_t *node_allocator = node_allocator_new();

    double end_second = time_second();
    double passed_second = end_second - start_second;
    printf("overhead of %d nodes: %.f ms\n",
           NODE_ALLOCATOR_BATCH_SIZE,
           passed_second * 1000);

    stack_t *stack_1 = stack_new();
    stack_t *stack_2 = stack_new();
    node_allocator_add_per_thread_stack(node_allocator, stack_1);
    node_allocator_add_per_thread_stack(node_allocator, stack_2);
    assert(node_allocator_thread_count(node_allocator) == 2);

    node_t *node = node_allocator_allocate(node_allocator, stack_1);
    assert(node);
    node_allocator_recycle(node_allocator, stack_1, &node);

    node_allocator_destroy(&node_allocator);

    printf("</node_test>\n");
}
