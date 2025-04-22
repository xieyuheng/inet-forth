#include "index.h"

void
check_name_not_defined(
    const worker_t *worker,
    const char *name,
    const token_t *token
) {
    mod_t *mod = worker->mod;
    value_t found = mod_find(mod, name);
    if (found) {
        who_printf("can not re-define name: %s\n", name);
        who_printf("path: "); path_relative_cwd_print(mod->path, stdout); printf("\n");
        who_printf("already defined to: "); value_print(found, stdout); printf("\n");
        code_print_context(stdout, mod->code, token->start, token->end);
        exit(1);
    }
}

void
check_name_defined(
    const worker_t *worker,
    const char *name,
    const token_t *token
) {
    mod_t *mod = worker->mod;
    const def_t *found = mod_find(mod, name);
    if (!found) {
        who_printf("[compiler-error] undefined name: %s\n", name);
        who_printf("path: "); path_relative_cwd_print(mod->path, stdout); printf("\n");
        code_print_context(stdout, mod->code, token->start, token->end);
        exit(1);
    }
}

void
check_node_name_defined(
    const worker_t *worker,
    const char *name,
    const token_t *token
) {
    mod_t *mod = worker->mod;
    value_t found = mod_find(mod, name);
    if (!found) {
        who_printf("undefined node name: %s\n", name);
        who_printf("path: "); path_relative_cwd_print(mod->path, stdout); printf("\n");
        code_print_context(stdout, mod->code, token->start, token->end);
        exit(1);
    }

    if (!is_node_ctor(found)) {
        who_printf("expect name defined as node ctor\n");
        who_printf("path: "); path_relative_cwd_print(mod->path, stdout); printf("\n");
        who_printf("value: "); value_print(found, stdout); printf("\n");
        code_print_context(stdout, mod->code, token->start, token->end);
        exit(1);
    }
}

void
check_port_name_defined(
    const worker_t *worker,
    const char *node_name,
    const char *port_name,
    const token_t *token
) {
    check_node_name_defined(worker, node_name, token);
    mod_t *mod = worker->mod;
    value_t found = mod_find(mod, node_name);
    const node_ctor_t *ctor = as_node_ctor(found);
    for (size_t i = 0; i < ctor->arity; i++) {
        port_info_t *port_info = ctor->port_infos[i];
        if (string_equal(port_info->name, port_name)) return;
    }

    who_printf("undefined port name: %s\n", port_name);
    who_printf("for node name: %s\n", node_name);
    who_printf("path: "); path_relative_cwd_print(mod->path, stdout); printf("\n");
    code_print_context(stdout, mod->code, token->start, token->end);
    exit(1);
}
