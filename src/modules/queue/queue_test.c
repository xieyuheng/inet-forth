#include "index.h"

void
queue_test(void) {
    printf("<queue_test>\n");

    queue_test_single_thread();
    queue_test_multi_thread();
    queue_test_throughput();

    printf("</queue_test>\n");
}
