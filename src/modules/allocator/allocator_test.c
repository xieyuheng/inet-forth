#include "index.h"

void
allocator_test(void) {
    printf("<allocator_test>\n");

    size_t expected_size = 100;
    allocator_t *allocator = allocator_new(expected_size);

    //

    allocator_destroy(&allocator);

    printf("</allocator_test>\n");
}
