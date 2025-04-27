#include "index.h"

void
test_allocator(void) {
    test_start();

    test_allocator_throughput();
    test_allocator_thread_safe();

    test_end();
}
