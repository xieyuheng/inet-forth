#include "index.h"

void
node_test(void) {
    printf("<node_test>\n");

    node_test_node_allocator_overhead();
    node_test_node_allocator_throughput();
    node_test_allocated_node_iter();

    printf("</node_test>\n");
}
