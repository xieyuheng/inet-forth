#pragma once

// a multiple producer multiple consumer
// thread safe double ended queue.

deque_t *deque_new(void);
void deque_destroy(deque_t **self_pointer);
