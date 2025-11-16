#include "index.h"

void
test_node_allocator_overhead(void) {
    test_start();

    double start_second = time_second();

    node_allocator_t *node_allocator = node_make_allocator();
    who_printf("overhead of %d nodes: %f ms\n",
                NODE_ALLOCATOR_BATCH_SIZE,
                time_passed_second(start_second) * 1000);

    node_allocator_destroy(&node_allocator);

    test_end();
}
