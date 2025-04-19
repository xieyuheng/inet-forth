#include "index.h"

// TODO just use list + lock for now

struct deque_t {
    spinlock_t *spinlock;
    list_t *list;
};

deque_t *
deque_new(void) {
    deque_t *self = new(deque_t);
    self->spinlock = spinlock_new();
    self->list = list_new();
    return self;
}

void
deque_destroy(deque_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    deque_t *self = *self_pointer;
    spinlock_destroy(&self->spinlock);
    list_destroy(&self->list);
    free(self);
    *self_pointer = NULL;
}

size_t
deque_length(deque_t *self) {
    spinlock_lock(self->spinlock);
    size_t length = list_length(self->list);
    spinlock_unlock(self->spinlock);
    return length;
}

bool
deque_is_empty(deque_t *self) {
    spinlock_lock(self->spinlock);
    bool is_empty = list_is_empty(self->list);
    spinlock_unlock(self->spinlock);
    return is_empty;
}

void
deque_push_front(deque_t *self, void *value) {
    spinlock_lock(self->spinlock);
    list_unshift(self->list, value);
    spinlock_unlock(self->spinlock);
}

void *
deque_pop_front(deque_t *self) {
    spinlock_lock(self->spinlock);
    void *value = list_shift(self->list);
    spinlock_unlock(self->spinlock);
    return value;
}

void
deque_push_back(deque_t *self, void *value) {
    spinlock_lock(self->spinlock);
    list_push(self->list, value);
    spinlock_unlock(self->spinlock);
}

void *deque_pop_back(deque_t *self) {
    spinlock_lock(self->spinlock);
    void *value = list_pop(self->list);
    spinlock_unlock(self->spinlock);
    return value;
}
