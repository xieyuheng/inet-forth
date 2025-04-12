#include "index.h"

void
node_test(void) {
    test_start();

    node_test_node_allocator_overhead();
    node_test_node_allocator_throughput();
    node_test_allocated_node_iter();

    test_end();
}
