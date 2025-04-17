#include "index.h"

// TODO just use list + lock for now

struct deque_t {
    mutex_t *mutex;
    list_t *list;
};

deque_t *
deque_new(void) {
    deque_t *self = new(deque_t);
    self->mutex = mutex_new();
    self->list = list_new();
    return self;
}

void
deque_destroy(deque_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    deque_t *self = *self_pointer;
    mutex_destroy(&self->mutex);
    list_destroy(&self->list);
    free(self);
    *self_pointer = NULL;
}
