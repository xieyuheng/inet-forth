#pragma once

node_allocator_t *node_allocator_new(size_t size);
void node_allocator_destroy(node_allocator_t **self_pointer);
