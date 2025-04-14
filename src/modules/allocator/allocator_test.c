#include "index.h"

void
allocator_test(void) {
    test_start();

    allocator_test_throughput();
    allocator_test_thread_safe();

    test_end();
}
