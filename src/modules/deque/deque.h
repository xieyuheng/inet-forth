#pragma once

// a multiple producer multiple consumer
// thread safe double ended queue.

deque_t *deque_new(void);
void deque_destroy(deque_t **self_pointer);

size_t deque_length(deque_t *self);
bool deque_is_empty(deque_t *self);

void deque_front_push(deque_t *self, void *value);
void *deque_front_pop(deque_t *self);

void deque_back_push(deque_t *self, void *value);
void *deque_back_pop(deque_t *self);
