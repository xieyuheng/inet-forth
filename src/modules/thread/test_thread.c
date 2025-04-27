#include "index.h"

void
test_thread(void) {
    test_start();

    test_thread_start();
    test_thread_mutex();
    test_thread_counter_non_atomic();
    test_thread_counter_atomic();
    test_thread_counter_stat();
    test_thread_counter_stat_eventual();
    test_thread_count_to_big();
    test_thread_weak_memory_dekker();
    test_thread_weak_memory_dekker_relaxed();
    test_thread_weak_memory_dekker_atomic();

    test_end();
}
