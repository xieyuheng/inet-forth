#pragma once

// growable array

array_t *array_new(size_t size);
void array_purge(array_t *self);
void array_destroy(array_t **self_pointer);

void array_set_destroy_fn(array_t *self, destroy_fn_t *destroy_fn);
array_t *array_new_with(size_t size, destroy_fn_t *destroy_fn);

#define ARRAY_AUTO_SIZE 64

array_t *array_new_auto(void);
array_t *array_new_auto_with(destroy_fn_t *destroy_fn);

size_t array_size(const array_t *self);
size_t array_grow_step(const array_t *self);
void array_set_grow_step(array_t *self, size_t grow_step);
size_t array_length(const array_t *self);
bool array_is_empty(const array_t *self);
bool array_is_full(const array_t *self);

void array_resize(array_t *self, size_t larger_size);

void *array_top(array_t *self);
void *array_pop(array_t *self);
void array_push(array_t *self, void *value);

void *array_get(const array_t *self, size_t index);
void *array_pick(const array_t *self, size_t back_index);

void array_set(array_t *self, size_t index, void *value);
