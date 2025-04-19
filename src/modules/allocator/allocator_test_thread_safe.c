#include "index.h"

#define CACHE_SIZE 1000
#define REPEATION_COUNT 1000

static void *
thread_fn(void *arg) {
    size_t value_count = 0;
    allocator_t *allocator = arg;
    stack_t *stack = stack_new();

    stack_t *allocated_stack = stack_new();
    for (size_t r = 0; r < REPEATION_COUNT; r++) {
        size_t batch_size = (size_t) rand() % (CACHE_SIZE * 2);
        value_count += batch_size;

        for (size_t i = 0; i < batch_size; i++) {
            char *abc = allocator_allocate(allocator, stack);
            assert(abc[0] == 'a');
            assert(abc[1] == 'b');
            assert(abc[2] == 'c');
            abc[0] = 'A';
            abc[1] = 'B';
            abc[2] = 'C';
            stack_push(allocated_stack, abc);
        }

        for (size_t i = 0; i < batch_size; i++) {
            char *abc = stack_pop(allocated_stack);
            assert(abc[0] == 'A');
            assert(abc[1] == 'B');
            assert(abc[2] == 'C');
            abc[0] = 'a';
            abc[1] = 'b';
            abc[2] = 'c';
            allocator_recycle(allocator, stack, (void **) &abc);
        }
    }

    return (void *) value_count;
}

void
allocator_test_thread_safe(void) {
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

    size_t value_count = 0;
    for (size_t i = 0; i < thread_count; i++) {
        tid_t tid = (tid_t) array_pop(thread_array);
        value_count += (size_t) thread_wait(tid);
    }


    who_printf("thread_count: %lu\n", thread_count);
    double throughput = value_count / 1000 / time_passed_second(start_second);
    who_printf("throughput: %.f k/s\n", throughput);

    allocator_destroy(&allocator);

    test_end();
}
