#include "index.h"

#define CACHE_SIZE 1000
#define BATCH_SIZE 1000
#define REPEATION_COUNT 5000

static void *
thread_fn(void *arg) {
    allocator_t *allocator = arg;
    stack_t *stack = stack_new();

    stack_t *allocated_stack = stack_new();
    for (size_t r = 0; r < REPEATION_COUNT; r++) {
        for (size_t i = 0; i < BATCH_SIZE; i++) {
            void *value = allocator_allocate(allocator, stack);
            stack_push(allocated_stack, value);
        }

        for (size_t i = 0; i < BATCH_SIZE; i++) {
            void *value = stack_pop(allocated_stack);
            allocator_recycle(allocator, stack, &value);
        }
    }

    return NULL;
}

void
test_allocator_throughput(void) {
    test_start();

    allocator_t *allocator = allocator_new(CACHE_SIZE);
    size_t ENOUGH_ALLOCATION_COUNT = CACHE_SIZE * 100;
    for (size_t i = 0; i < ENOUGH_ALLOCATION_COUNT; i++) {
        stack_push(allocator->stack, string_copy("abc"));
    }

    double start_second = time_second();

    size_t thread_count = 10;
    array_t *thread_array = array_new_auto();
    for (size_t i = 0; i < thread_count; i++) {
        tid_t tid = thread_start(thread_fn, allocator);
        array_push(thread_array, (void *) tid);
    }

    for (size_t i = 0; i < thread_count; i++) {
        tid_t tid = (tid_t) array_pop(thread_array);
        thread_wait(tid);
    }

    who_printf("thread_count: %lu\n", thread_count);
    double throughput = REPEATION_COUNT * BATCH_SIZE / 1000 / time_passed_second(start_second);
    who_printf("throughput: %.f k/s\n", throughput);

    allocator_destroy(&allocator);

    test_end();
}
