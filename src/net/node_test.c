#include "index.h"

void
node_test(void) {
    printf("<node_test>\n");

    double start_second = time_second();

    node_allocator_t *node_allocator = node_allocator_new(NODE_COUNT);
    node_allocator_destroy(&node_allocator);

    double end_second = time_second();
    double passed_second = end_second - start_second;
    printf("passed_second: %.f\n", passed_second);

    printf("</node_test>\n");
}
