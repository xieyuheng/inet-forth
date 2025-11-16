#pragma once

fast_spinlock_t *make_fast_spinlock(void);
void fast_spinlock_destroy(fast_spinlock_t **self_pointer);

void fast_spinlock_lock(fast_spinlock_t *self);
bool fast_spinlock_try_lock(fast_spinlock_t *self);
void fast_spinlock_unlock(fast_spinlock_t *self);
