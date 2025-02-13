#pragma once

path_t *path_new(const char *string);
void path_destroy(path_t **self_pointer);

bool path_is_relative(const path_t *self);
bool path_is_absolute(const path_t *self);

path_t *path_copy(path_t *self);
bool path_equal(path_t *x, path_t *y);

void path_join(path_t *self, const char *string);
const char *path_string(path_t *self);
