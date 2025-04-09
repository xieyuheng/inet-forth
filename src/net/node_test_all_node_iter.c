#include "index.h"

void
node_test_all_node_iter(void) {
    printf("<node_test_all_node_iter>\n");

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

    all_node_iter_t *all_node_iter = all_node_iter_new(node_allocator);
    node_t *node = all_node_iter_first(all_node_iter);
    size_t node_count = 0;
    while (node) {
        node_print(node, stdout);
        printf("\n");
        node_count++;
        assert(list_find(node_list, node));
        node = all_node_iter_next(all_node_iter);
    }

    assert(node_count == list_length(node_list));

    node_allocator_destroy(&node_allocator);

    printf("</node_test_all_node_iter>\n");
}
