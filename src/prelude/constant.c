#include "index.h"

void x_define_constant(worker_t *worker) {
    token_t *token = list_shift(worker->token_list);
    value_t value = stack_pop(worker->value_stack);
    define_constant(worker->mod, token->string, value);
    token_destroy(&token);
}
