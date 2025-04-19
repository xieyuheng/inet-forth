#include "index.h"

// TODO just use list + lock for now

struct deque_t {
    fast_spinlock_t *fast_spinlock;
    list_t *list;
};

deque_t *
deque_new(void) {
    deque_t *self = new(deque_t);
    self->fast_spinlock = fast_spinlock_new();
    self->list = list_new();
    return self;
}

void
deque_destroy(deque_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    deque_t *self = *self_pointer;
    fast_spinlock_destroy(&self->fast_spinlock);
    list_destroy(&self->list);
    free(self);
    *self_pointer = NULL;
}

size_t
deque_length(deque_t *self) {
    fast_spinlock_lock(self->fast_spinlock);
    size_t length = list_length(self->list);
    fast_spinlock_unlock(self->fast_spinlock);
    return length;
}

bool
deque_is_empty(deque_t *self) {
    fast_spinlock_lock(self->fast_spinlock);
    bool is_empty = list_is_empty(self->list);
    fast_spinlock_unlock(self->fast_spinlock);
    return is_empty;
}

void
deque_push_front(deque_t *self, void *value) {
    fast_spinlock_lock(self->fast_spinlock);
    list_unshift(self->list, value);
    fast_spinlock_unlock(self->fast_spinlock);
}

void *
deque_pop_front(deque_t *self) {
    fast_spinlock_lock(self->fast_spinlock);
    void *value = list_shift(self->list);
    fast_spinlock_unlock(self->fast_spinlock);
    return value;
}

void
deque_push_back(deque_t *self, void *value) {
    fast_spinlock_lock(self->fast_spinlock);
    list_push(self->list, value);
    fast_spinlock_unlock(self->fast_spinlock);
}

void *deque_pop_back(deque_t *self) {
    fast_spinlock_lock(self->fast_spinlock);
    void *value = list_pop(self->list);
    fast_spinlock_unlock(self->fast_spinlock);
    return value;
}
