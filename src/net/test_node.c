#include "index.h"

void
test_node(void) {
    test_start();

    test_node_allocator_overhead();
    test_node_allocator_throughput();
    test_node_allocated_node_iter();

    test_end();
}
