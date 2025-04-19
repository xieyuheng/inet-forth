#include "index.h"

// TODO just use list + lock for now

struct deque_t {
    mini_spinlock_t *mini_spinlock;
    list_t *list;
};

deque_t *
deque_new(void) {
    deque_t *self = new(deque_t);
    self->mini_spinlock = mini_spinlock_new();
    self->list = list_new();
    return self;
}

void
deque_destroy(deque_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    deque_t *self = *self_pointer;
    mini_spinlock_destroy(&self->mini_spinlock);
    list_destroy(&self->list);
    free(self);
    *self_pointer = NULL;
}

size_t
deque_length(deque_t *self) {
    mini_spinlock_lock(self->mini_spinlock);
    size_t length = list_length(self->list);
    mini_spinlock_unlock(self->mini_spinlock);
    return length;
}

bool
deque_is_empty(deque_t *self) {
    mini_spinlock_lock(self->mini_spinlock);
    bool is_empty = list_is_empty(self->list);
    mini_spinlock_unlock(self->mini_spinlock);
    return is_empty;
}

void
deque_push_front(deque_t *self, void *value) {
    mini_spinlock_lock(self->mini_spinlock);
    list_unshift(self->list, value);
    mini_spinlock_unlock(self->mini_spinlock);
}

void *
deque_pop_front(deque_t *self) {
    mini_spinlock_lock(self->mini_spinlock);
    void *value = list_shift(self->list);
    mini_spinlock_unlock(self->mini_spinlock);
    return value;
}

void
deque_push_back(deque_t *self, void *value) {
    mini_spinlock_lock(self->mini_spinlock);
    list_push(self->list, value);
    mini_spinlock_unlock(self->mini_spinlock);
}

void *deque_pop_back(deque_t *self) {
    mini_spinlock_lock(self->mini_spinlock);
    void *value = list_pop(self->list);
    mini_spinlock_unlock(self->mini_spinlock);
    return value;
}
