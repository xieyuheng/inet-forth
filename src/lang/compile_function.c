#include "index.h"

function_t *
compile_function(worker_t *worker) {
    function_t *function = function_new();
    while (true) {
        assert(list_length(worker->token_list) > 0);
        token_t *token = list_first(worker->token_list);
        if (string_equal(token->string, "end")) {
            (void) list_shift(worker->token_list);
            token_destroy(&token);
            function_build(function);
            return function;
        }

        compile_one(worker, function);
    }
}
