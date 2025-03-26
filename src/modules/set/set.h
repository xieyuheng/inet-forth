#pragma once

set_t *set_new(void);
void set_destroy(set_t **self_pointer);

void set_set_hash_fn(set_t *self, hash_fn_t *hash_fn);
void set_set_destroy_fn(set_t *self, destroy_fn_t *destroy_fn);
void set_set_equal_fn(set_t *self, equal_fn_t *equal_fn);

set_t *set_new_with(destroy_fn_t *destroy_fn);
set_t *string_set_new(void);

size_t set_length(const set_t *self);

// add successes if the value is not already exist.
bool set_add(set_t *self, void *value);

// put auto destroy old value if there is destroy_fn
void set_put(set_t *self, void *value);

bool set_has(set_t *self, void *value);

// return successful deleted or not.
bool set_delete(set_t *self, void *value);

void *set_first(set_t *self);
void *set_next(set_t *self);

list_t *set_to_list(set_t *self);
array_t *set_to_array(set_t *self);
