#pragma once

path_t *path_new(const char *string);
void path_destroy(path_t **self_pointer);

path_t *path_new_cwd(void);

bool path_is_relative(const path_t *self);
bool path_is_absolute(const path_t *self);

path_t *path_copy(path_t *self);
bool path_equal(path_t *x, path_t *y);

void path_join(path_t *self, const char *string);
const char *path_string(path_t *self);

path_t *path_relative(path_t *from, path_t *to);
void path_relative_print(path_t *from, path_t *to, file_t *file);
void path_relative_cwd_print(path_t *to, file_t *file);
