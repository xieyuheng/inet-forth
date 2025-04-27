#include "index.h"

void
test_queue(void) {
    test_start();

    test_queue_single_thread();
    test_queue_multi_thread();
    test_queue_throughput();

    test_end();
}
