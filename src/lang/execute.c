#include "index.h"

static bool
execute_int(worker_t *worker) {
    token_t *token = list_first(worker->token_list);
    if (token->kind != INT_TOKEN) return false;
    if (!string_is_xint(token->string)) return false;
    (void) list_shift(worker->token_list);

    value_t value = xint(string_parse_xint(token->string));
    stack_push(worker->value_stack, value);
    token_destroy(&token);
    return true;
}

static bool
execute_float(worker_t *worker) {
    token_t *token = list_first(worker->token_list);
    if (token->kind != FLOAT_TOKEN) return false;
    if (!string_is_double(token->string)) return false;
    (void) list_shift(worker->token_list);

    value_t value = xfloat(string_parse_double(token->string));
    stack_push(worker->value_stack, value);
    token_destroy(&token);
    return true;
}

static bool
execute_generic(worker_t *worker) {
    token_t *token = list_first(worker->token_list);
    if (token->kind != GENERIC_TOKEN) return false;

    function_t *function = function_new();
    compile_one(worker, function);

    size_t base_length = stack_length(worker->return_stack);
    frame_t *frame = frame_new(function);
    stack_push(worker->return_stack, frame);
    worker_run_until(worker, base_length);

    function_destroy(&function);
    return true;
}

void
execute_one(worker_t *worker) {
    if (execute_int(worker)) return;
    if (execute_float(worker)) return;
    if (execute_generic(worker)) return;

    token_t *token = list_first(worker->token_list);
    fprintf(stderr, "[execute_one] unknown token: %s\n", token->string);
}

void
execute_all(worker_t *worker) {
    while (!list_is_empty(worker->token_list)) {
        execute_one(worker);
    }
}
