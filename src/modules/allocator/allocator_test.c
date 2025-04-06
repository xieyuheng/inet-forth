#include "index.h"

#define EXPECTED_ALLOCATION_COUNT 1000
#define ACTUAL_ALLOCATION_COUNT 500
#define REPEATION_COUNT 10000

static void *
thread_fn(void *arg) {
    allocator_t *allocator = arg;
    stack_t *value_stack = stack_new();
    stack_t *allocated_value_stack = stack_new();
    for (size_t r = 0; r < REPEATION_COUNT; r++) {
        for (size_t i = 0; i < ACTUAL_ALLOCATION_COUNT; i++) {
            char *s = allocator_allocate(allocator, value_stack);
            assert(string_equal(s, "abc"));
            stack_push(allocated_value_stack, s);
        }

        for (size_t i = 0; i < ACTUAL_ALLOCATION_COUNT; i++) {
            char *s = stack_pop(allocated_value_stack);
            assert(string_equal(s, "abc"));
            allocator_free(allocator, value_stack, s);
        }
    }

    return NULL;
}

void
allocator_test(void) {
    printf("<allocator_test>\n");

    allocator_t *allocator = allocator_new(EXPECTED_ALLOCATION_COUNT);

    stack_t *value_stack = allocator_value_stack(allocator);
    for (size_t i = 0; i < EXPECTED_ALLOCATION_COUNT * 10; i++) {
        stack_push(value_stack, string_copy("abc"));
    }

    thread_id_t thread_id_1 = thread_start(thread_fn, allocator);
    thread_id_t thread_id_2 = thread_start(thread_fn, allocator);

    thread_wait(thread_id_1);
    thread_wait(thread_id_2);

    allocator_destroy(&allocator);

    printf("</allocator_test>\n");
}
