#pragma once

// thread safe double ended queue

// TODO just use list + lock for now

struct deque_t {
    list_t *list;
};

deque_t *deque_new(void);
