#include "index.h"

#define EXPECTED_ALLOCATION_COUNT 1000
#define ACTUAL_ALLOCATION_COUNT 1000
#define REPEATION_COUNT 10000

static void *
thread_fn(void *arg) {
    allocator_t *allocator = arg;
    stack_t *value_stack = stack_new();
    stack_t *allocated_value_stack = stack_new();
    for (size_t r = 0; r < REPEATION_COUNT; r++) {
        for (size_t i = 0; i < ACTUAL_ALLOCATION_COUNT; i++) {
            void *value = allocator_allocate(allocator, value_stack);
            stack_push(allocated_value_stack, value);
        }

        for (size_t i = 0; i < ACTUAL_ALLOCATION_COUNT; i++) {
            void *value = stack_pop(allocated_value_stack);
            allocator_free(allocator, value_stack, value);
        }
    }

    return NULL;
}

void
allocator_test(void) {
    printf("<allocator_test>\n");

    allocator_t *allocator = allocator_new(EXPECTED_ALLOCATION_COUNT);

    stack_t *value_stack = allocator_value_stack(allocator);
    size_t enough_allocation_count = EXPECTED_ALLOCATION_COUNT * 100;
    for (size_t i = 0; i < enough_allocation_count; i++) {
        stack_push(value_stack, string_copy("abc"));
    }

    double start_second = time_second();

    thread_id_t thread_id_1 = thread_start(thread_fn, allocator);
    thread_id_t thread_id_2 = thread_start(thread_fn, allocator);

    thread_wait(thread_id_1);
    thread_wait(thread_id_2);

    double end_second = time_second();
    double passed_second = end_second - start_second;
    double throughput = REPEATION_COUNT * ACTUAL_ALLOCATION_COUNT
        / 1000 / passed_second;
    printf("throughput: %.f k/s\n", throughput);

    allocator_destroy(&allocator);

    printf("</allocator_test>\n");
}
