#include "index.h"

void x_define_function(worker_t *worker) {
    token_t *token = list_shift(worker->token_list);
    function_t *function = compile_function(worker);
    define_function(worker->mod, token->string, function);
    token_destroy(&token);
}

static void
import(worker_t *worker, list_t *name_list, path_t *target_path) {
    path_t *path = path_copy(worker->mod->path);
    path_join(path, "..");
    path_join(path, path_string(target_path));
    mod_t *imported_mod = load_mod(path);
    char *name = list_first(name_list);
    while (name) {
        value_t value = mod_find(imported_mod, name);
        if (value == NULL) {
            who_printf("unknown name: %s", name);
            exit(1);
        }

        define(worker->mod, name, value);
        name = list_next(name_list);
    }

    return;
}

void x_import(worker_t *worker) {
    token_t *token = list_shift(worker->token_list);
    list_t *name_list = string_make_list();
    while (true) {
        if (!token) {
            who_printf("expect a path to be the end of import\n");
            exit(1);
        } else if (token->kind == GENERIC_TOKEN) {
            list_push(name_list, string_copy(token->string));
            token_destroy(&token);
        } else if (token->kind == STRING_TOKEN) {
            path_t *path = make_path(token->string);
            token_destroy(&token);
            import(worker, name_list, path);
            list_destroy(&name_list);
            path_destroy(&path);
            return;
        } else {
            who_printf("can not handle token: %s\n", token->string);
            exit(1);
        }

        token = list_shift(worker->token_list);
    }
}
