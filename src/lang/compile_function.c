#include "index.h"

function_t *
compile_function(vm_t *vm) {
    function_t *function = function_new();
    while (true) {
        assert(list_length(vm->token_list) > 0);
        token_t *token = list_first(vm->token_list);
        if (string_equal(token->string, "end")) {
            (void) list_shift(vm->token_list);
            token_destroy(&token);
            function_build(function);
            return function;
        }

        compile_one(vm, function);
    }
}
