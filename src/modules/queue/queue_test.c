#include "index.h"

void
queue_test(void) {
    test_start();

    queue_test_single_thread();
    queue_test_multi_thread();
    queue_test_throughput();

    test_end();
}
