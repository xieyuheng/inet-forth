#pragma once

queue_t *queue_new(size_t size);
void queue_purge(queue_t *self);
void queue_destroy(queue_t **self_pointer);

void queue_set_destroy_fn(queue_t *self, destroy_fn_t *destroy_fn);
queue_t *queue_new_with(size_t size, destroy_fn_t *destroy_fn);

size_t queue_size(const queue_t *self);
size_t queue_length(const queue_t *self);
bool queue_is_full(const queue_t *self);
bool queue_is_empty(const queue_t *self);

// return successful or not
bool queue_back_push(queue_t *self, void *value);
void *queue_front_pop(queue_t *self);

// NOT thread safe
void *queue_get(const queue_t *self, size_t index);
