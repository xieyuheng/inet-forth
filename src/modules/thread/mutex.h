#pragma once

mutex_t *mutex_new(void);
void mutex_destroy(mutex_t **self_pointer);

void mutex_lock(mutex_t *self);
bool mutex_try_lock(mutex_t *self);
void mutex_unlock(mutex_t *self);
