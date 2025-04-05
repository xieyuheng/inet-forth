#include "index.h"

void
allocator_test(void) {
    printf("<allocator_test>\n");

    allocator_t *allocator = allocator_new();

    //

    allocator_destroy(&allocator);

    printf("</allocator_test>\n");
}
