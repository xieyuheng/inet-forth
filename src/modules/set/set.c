#include "index.h"

struct set_t {
    hash_t *value_hash;
};

set_t *
set_new(void) {
    set_t *self = new(set_t);
    self->value_hash = hash_new();
    return self;
}

void
set_destroy(set_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        set_t *self = *self_pointer;
        hash_destroy(&self->value_hash);
        free(self);
        *self_pointer = NULL;
    }
}

void
set_set_hash_fn(set_t *self, hash_fn_t *hash_fn) {
    hash_set_hash_fn(self->value_hash, hash_fn);
}

void
set_set_destroy_fn(set_t *self, destroy_fn_t *destroy_fn) {
    // key is the same as value
    hash_set_key_destroy_fn(self->value_hash, destroy_fn);
}

void
set_set_equal_fn(set_t *self, equal_fn_t *equal_fn) {
    hash_set_key_equal_fn(self->value_hash, equal_fn);
}

set_t *
set_new_with(destroy_fn_t *destroy_fn) {
    set_t *self = set_new();
    set_set_destroy_fn(self, destroy_fn);
    return self;
}

set_t *
string_set_new(void) {
    set_t *self = set_new();
    set_set_hash_fn(self, (hash_fn_t *) string_bernstein_hash);
    set_set_destroy_fn(self, (destroy_fn_t *) string_destroy);
    set_set_equal_fn(self, (equal_fn_t *) string_equal);
    return self;
}

size_t
set_length(const set_t *self) {
    return hash_length(self->value_hash);
}

bool
set_add(set_t *self, void *value) {
    return hash_set(self->value_hash, value, value);
}

void
set_put(set_t *self, void *value) {
    hash_put(self->value_hash, value, value);
    return;
}

bool
set_has(set_t *self, void *value) {
    return hash_has(self->value_hash, value);
}

bool
set_delete(set_t *self, void *value) {
    return hash_delete(self->value_hash, value);
}

void *
set_first(set_t *self) {
    return hash_first(self->value_hash);
}

void *
set_next(set_t *self) {
    return hash_next(self->value_hash);
}

list_t *
set_to_list(set_t *self) {
    list_t *list = list_new();
    void *value = set_first(self);
    while (value) {
        list_push(list, value);
        value = set_next(self);
    }

    return list;
}

array_t *
set_to_array(set_t *self) {
    array_t *array = array_auto();
    void *value = set_first(self);
    while (value) {
        array_push(array, value);
        value = set_next(self);
    }

    return array;
}
