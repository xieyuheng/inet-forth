#include "index.h"

void
thread_test(void) {
    printf("<thread_test>\n");

    thread_test_start();
    thread_test_mutex();
    thread_test_counter_non_atomic();
    thread_test_counter_atomic();
    thread_test_counter_stat();
    thread_test_counter_stat_eventual();
    thread_test_count_to_big();
    thread_test_weak_memory_dekker();
    thread_test_weak_memory_dekker_relaxed();
    thread_test_weak_memory_dekker_atomic();

    printf("</thread_test>\n");
}
