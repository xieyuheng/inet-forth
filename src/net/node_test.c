#include "index.h"

void
node_test(void) {
    printf("<node_test>\n");

    double start_second = time_second();

    node_allocator_t *node_allocator = node_allocator_new(NODE_COUNT);

    stack_t *stack_1 = stack_new();
    stack_t *stack_2 = stack_new();
    node_allocator_add_per_thread_stack(node_allocator, stack_1);
    node_allocator_add_per_thread_stack(node_allocator, stack_2);
    assert(node_allocator_thread_count(node_allocator) == 2);

    node_allocator_destroy(&node_allocator);

    double end_second = time_second();
    double passed_second = end_second - start_second;
    printf("passed_second: %.f\n", passed_second);

    printf("</node_test>\n");
}
