#include "index.h"

void
allocator_test(void) {
    printf("<allocator_test>\n");

    size_t expected_allocation_count = 100;
    allocator_t *allocator = allocator_new(expected_allocation_count);

    //

    allocator_destroy(&allocator);

    printf("</allocator_test>\n");
}
