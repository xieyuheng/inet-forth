#include "index.h"

deque_t *
deque_new(void) {
    deque_t *self = new(deque_t);
    self->list = list_new();
    return self;
}
