#include "index.h"

void
node_test_allocated_node_iter(void) {
    test_start();

    node_allocator_t *node_allocator = node_allocator_new();

    size_t per_thread_node_count = 10;
    list_t *node_list = list_new();


    {
        stack_t *stack = stack_new();
        for (size_t i = 0; i < per_thread_node_count; i++) {
            node_t *node = node_allocator_allocate(node_allocator, stack);
            list_push(node_list, node);
        }
    }

    {
        stack_t *stack = stack_new();
        for (size_t i = 0; i < per_thread_node_count; i++) {
            node_t *node = node_allocator_allocate(node_allocator, stack);
            list_push(node_list, node);
        }
    }

    allocated_node_iter_t *allocated_node_iter = allocated_node_iter_new(node_allocator);
    node_t *node = allocated_node_iter_first(allocated_node_iter);
    size_t node_count = 0;
    while (node) {
        node_print(node, stdout);
        printf("\n");
        node_count++;
        assert(list_find(node_list, node));
        node = allocated_node_iter_next(allocated_node_iter);
    }

    assert(node_count == list_length(node_list));

    node_allocator_destroy(&node_allocator);

    test_end();
}
