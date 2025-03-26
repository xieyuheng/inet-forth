#include "index.h"

struct array_t {
    size_t size;
    size_t grow_step;
    size_t cursor;
    void **values;
    destroy_fn_t *destroy_fn;
};

array_t *
array_new(size_t size) {
    array_t *self = new(array_t);
    self->size = size;
    self->grow_step = size;
    self->cursor = 0;
    self->values = allocate_pointers(size);
    return self;
}

void
array_purge(array_t *self) {
    assert(self);
    while(!array_is_empty(self)) {
        void *value = array_pop(self);
        if (self->destroy_fn)
            self->destroy_fn(&value);
    }
}

void
array_destroy(array_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer) {
        array_t *self = *self_pointer;
        array_purge(self);
        free(self->values);
        free(self);
        *self_pointer = NULL;
    }
}

void
array_set_destroy_fn(array_t *self, destroy_fn_t *destroy_fn) {
    self->destroy_fn = destroy_fn;
}

array_t *
array_new_with(size_t size, destroy_fn_t *destroy_fn) {
    array_t *self = array_new(size);
    self->destroy_fn = destroy_fn;
    return self;
}

array_t *
array_auto(void) {
    return array_new(ARRAY_AUTO_SIZE);
}

array_t *
array_auto_with(destroy_fn_t *destroy_fn) {
    return array_new_with(ARRAY_AUTO_SIZE, destroy_fn);
}

size_t
array_size(const array_t *self) {
    return self->size;
}

size_t
array_grow_step(const array_t *self) {
    return self->grow_step;
}

void
array_set_grow_step(array_t *self, size_t grow_step) {
    self->grow_step = grow_step;
}

size_t
array_length(const array_t *self) {
    return self->cursor;
}

bool
array_is_empty(const array_t *self) {
    return self->cursor == 0;
}

bool
array_is_full(const array_t *self) {
    return self->cursor == self->size;
}

void
array_grow(array_t *self, size_t larger_size) {
    assert(larger_size >= self->size);
    if (larger_size == self->size) return;

    self->values = reallocate_pointers(self->values, self->size, larger_size);
    self->size = larger_size;
}

void *
array_top(array_t *self) {
    assert(self->cursor > 0);
    void *value = self->values[self->cursor - 1];
    return value;
}

void *
array_pop(array_t *self) {
    assert(self->cursor > 0);
    self->cursor--;
    void *value = self->values[self->cursor];
    return value;
}

void
array_push(array_t *self, void *value) {
    if (array_is_full(self)) {
        array_grow(self, self->size + self->grow_step);
    }

    self->values[self->cursor] = value;
    self->cursor++;
}

void *
array_get(const array_t *self, size_t index) {
    assert(index < self->size);
    return self->values[index];
}

void
array_set(array_t *self, size_t index, void *value) {
    assert(index < self->size);
    self->values[index] = value;
}

void *
array_pick(array_t *self, size_t index) {
    assert(index < self->size);
    return self->values[self->cursor - 1 - index];
}
