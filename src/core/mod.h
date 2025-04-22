#pragma once

// A mod is a compilation unit,
// like the dictionary of forth.

struct mod_t {
    path_t *path;
    char *code;
    hash_t *value_hash;
    // the worker that is used to load this mod,
    // owned by this mod.
    // - used by `player_start`
    worker_t *loader_worker;
};

mod_t *mod_new(path_t *path, char *code);
void mod_destroy(mod_t **self_pointer);

value_t mod_find(const mod_t *self, const char *name);
void mod_define_rule(mod_t *self, const char *first_name, const char *second_name, function_t *function);

void mod_print(const mod_t *self, file_t *file);
