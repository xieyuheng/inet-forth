#pragma once

// thread safe double ended queue

// TODO just use list + lock for now

struct deque_t {
    mutex_t *mutex;
    list_t *list;
};

deque_t *deque_new(void);
void deque_destroy(deque_t **self_pointer);
