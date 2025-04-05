#pragma once

mutex_t *mutex_new(void);
void mutex_destroy(mutex_t **self_pointer);
