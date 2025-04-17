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

size_t
deque_length(deque_t *self) {
    mutex_lock(self->mutex);
    size_t length = list_length(self->list);
    mutex_unlock(self->mutex);
    return length;
}

bool
deque_is_empty(deque_t *self) {
    mutex_lock(self->mutex);
    bool is_empty = list_is_empty(self->list);
    mutex_unlock(self->mutex);
    return is_empty;
}

void
deque_front_push(deque_t *self, void *value) {
    mutex_lock(self->mutex);
    list_unshift(self->list, value);
    mutex_unlock(self->mutex);
}

void *
deque_front_pop(deque_t *self) {
    mutex_lock(self->mutex);
    void *value = list_shift(self->list);
    mutex_unlock(self->mutex);
    return value;
}

void
deque_back_push(deque_t *self, void *value) {
    mutex_lock(self->mutex);
    list_push(self->list, value);
    mutex_unlock(self->mutex);
}

void *deque_back_pop(deque_t *self) {
    mutex_lock(self->mutex);
    void *value = list_pop(self->list);
    mutex_unlock(self->mutex);
    return value;
}
