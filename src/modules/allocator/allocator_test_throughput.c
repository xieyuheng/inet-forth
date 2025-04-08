#include "index.h"

#define CACHE_SIZE 1000
#define BATCH_SIZE 1000
#define REPEATION_COUNT 10000

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
            allocator_free(allocator, stack, value);
        }
    }

    return NULL;
}

void
allocator_test_throughput(void) {
    printf("<allocator_test_throughput>\n");

    allocator_t *allocator = allocator_new(CACHE_SIZE);

    stack_t *stack = allocator_stack(allocator);
    size_t ENOUGH_ALLOCATION_COUNT = CACHE_SIZE * 100;
    for (size_t i = 0; i < ENOUGH_ALLOCATION_COUNT; i++) {
        stack_push(stack, string_copy("abc"));
    }

    double start_second = time_second();

    thread_id_t T1 = thread_start(thread_fn, allocator);
    thread_id_t T2 = thread_start(thread_fn, allocator);

    thread_wait(T1);
    thread_wait(T2);

    double end_second = time_second();
    double passed_second = end_second - start_second;
    double throughput = REPEATION_COUNT * BATCH_SIZE / 1000 / passed_second;
    printf("throughput: %.f k/s\n", throughput);

    allocator_destroy(&allocator);

    printf("</allocator_test_throughput>\n");
}
