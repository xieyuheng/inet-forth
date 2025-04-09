#include "index.h"

void
node_test_node_allocator_overhead(void) {
    printf("<node_test_node_allocator_overhead>\n");

    double start_second = time_second();

    node_allocator_t *node_allocator = node_allocator_new();

    double end_second = time_second();
    double passed_second = end_second - start_second;
    printf("overhead of %d nodes: %.f ms\n",
           NODE_ALLOCATOR_BATCH_SIZE,
           passed_second * 1000);

    node_allocator_destroy(&node_allocator);

    printf("</node_test_node_allocator_overhead>\n");
}
