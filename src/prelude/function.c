#include "index.h"

void x_define_function(worker_t *worker) {
    token_t *token = list_shift(worker->token_list);
    function_t *function = compile_function(worker);
    define_function(worker->mod, token->string, function);
    token_destroy(&token);
}
