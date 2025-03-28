#pragma once

struct node_t {
    const node_ctor_t *ctor;
    size_t id;
    wire_t **ports;
};

node_t *node_new(const node_ctor_t *ctor, size_t id);
void node_destroy(node_t **self_pointer);

void node_print(const node_t *self, file_t *file);
