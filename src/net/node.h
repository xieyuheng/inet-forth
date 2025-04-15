#pragma once

struct node_t {
    const node_ctor_t *ctor;
    mutex_t *mutex;
    size_t id;
    bool is_allocated;
    array_t *value_array;
};

node_t *node_new(void);
void node_destroy(node_t **self_pointer);

void node_set_value(node_t *self, size_t index, value_t value);
value_t node_get_value(const node_t *self, size_t index);
port_info_t *node_get_port_info(const node_t *self, size_t index);

void node_print_name(const node_t *self, file_t *file);
void node_print(const node_t *self, file_t *file);
void node_print_connected(node_t *self, hash_t *node_adjacency_hash, file_t *file);

bool node_has_wire(node_t *node, wire_t *wire);
