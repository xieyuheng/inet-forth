#include "index.h"

void
allocator_test(void) {
    test_start();

    allocator_test_throughput();

    test_end();
}
