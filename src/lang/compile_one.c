#include "index.h"

static bool
compile_int(vm_t *vm, function_t *function) {
    token_t *token = list_first(vm->token_list);
    if (token->kind != INT_TOKEN) return false;
    if (!string_is_xint(token->string)) return false;
    (void) list_shift(vm->token_list);

    value_t value = xint(string_parse_xint(token->string));
    function_add_op(function, op_literal(value));
    token_destroy(&token);
    return true;
}

static bool
compile_float(vm_t *vm, function_t *function) {
    token_t *token = list_first(vm->token_list);
    if (token->kind != FLOAT_TOKEN) return false;
    if (!string_is_double(token->string)) return false;
    (void) list_shift(vm->token_list);

    value_t value = xfloat(string_parse_double(token->string));
    function_add_op(function, op_literal(value));
    token_destroy(&token);
    return true;
}

static bool
compile_bind(vm_t *vm, function_t *function) {
    function_ctx_t *ctx = function->ctx;

    token_t *token = list_first(vm->token_list);
    if (!string_equal(token->string, "(")) return false;
    (void) list_shift(vm->token_list);
    token_destroy(&token);

    list_t *local_token_list = list_new();
    while (true) {
        token_t *token = list_first(vm->token_list);
        if (string_equal(token->string, ")")) {
            (void) list_shift(vm->token_list);
            token_destroy(&token);
            break;
        }

        (void) list_shift(vm->token_list);
        list_push(local_token_list, token);
    }

    size_t index = hash_length(ctx->local_index_hash);
    while (!list_is_empty(local_token_list)) {
        token_t *token = list_pop(local_token_list);
        if (hash_has(ctx->local_index_hash, token->string)) {
            size_t old_index = (size_t) hash_get(ctx->local_index_hash, token->string);
            function_add_op(function, op_set_variable(old_index));
        } else {
            hash_set(ctx->local_index_hash, token->string, (void *) index);
            function_add_op(function, op_set_variable(index));
            index++;
        }
    }

    list_destroy(&local_token_list);
    return true;
}

static bool
compile_generic(vm_t *vm, function_t *function) {
    function_ctx_t *ctx = function->ctx;
    token_t *token = list_first(vm->token_list);
    if (hash_has(ctx->local_index_hash, token->string)) {
        (void) list_shift(vm->token_list);
        size_t index = (size_t) hash_get(ctx->local_index_hash, token->string);
        function_add_op(function, op_get_variable(index));
        token_destroy(&token);
        return true;
    }

    const def_t *def = mod_find_def(vm->mod, token->string);
    if (def == NULL) {
        fprintf(stderr, "[compile_generic] undefined name: %s\n", token->string);
        exit(1);
    }

    (void) list_shift(vm->token_list);
    function_add_op(function, op_call(def));
    token_destroy(&token);
    return true;
}

void
compile_one(vm_t *vm, function_t *function) {
    if (compile_int(vm, function)) return;
    if (compile_float(vm, function)) return;
    if (compile_bind(vm, function)) return;
    if (compile_generic(vm, function)) return;

    token_t *token = list_first(vm->token_list);
    fprintf(stderr, "[compile_one] unknown token: %s\n", token->string);
}
