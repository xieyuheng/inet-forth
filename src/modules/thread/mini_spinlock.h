#pragma once

mini_spinlock_t *mini_spinlock_new(void);
void mini_spinlock_destroy(mini_spinlock_t **self_pointer);

void mini_spinlock_lock(mini_spinlock_t *self);
void mini_spinlock_unlock(mini_spinlock_t *self);
