#include "index.h"

void
x_connect(worker_t *worker) {
    value_t second = stack_pop(worker->value_stack);
    value_t first = stack_pop(worker->value_stack);
    
    assert(second);
    assert(first);

    worker_connect(worker, first, second);
}

void
x_link(worker_t *worker) {
    wire_t *wire = wire_new();

    stack_push(worker->value_stack, wire);
    stack_push(worker->value_stack, wire);
}

void
x_run(worker_t *worker) {
    run_task(worker);
}

static void
value_print_connected(worker_t *worker) {
    value_t value = stack_top(worker->value_stack);
    if (!value) {
        printf("[value_print_connected] expect top value\n");
        return;
    }

    node_t *node;

    if (is_wire(value)) {
        wire_t *wire = as_wire(value);
        node = worker_lookup_node_by_wire(worker, wire);
    }

    if (is_principal_wire(value)) {
        principal_wire_t *principal_wire = as_principal_wire(value);
        node = principal_wire->node;
    }

    assert(node);

    hash_t *node_adjacency_hash = build_node_adjacency_hash(worker->node_allocator);
    node_print_connected(node, node_adjacency_hash, stdout);
    hash_destroy(&node_adjacency_hash);
    fprintf(stdout, "\n");
}

void
x_inspect_run(worker_t *worker) {
    value_print_connected(worker);
    run_task(worker);
    value_print_connected(worker);
}

static void
define_node(mod_t *mod, const char *name, list_t *input_token_list, list_t *output_token_list) {
    size_t input_arity = list_length(input_token_list);
    size_t output_arity = list_length(output_token_list);
    node_ctor_t *node_ctor = node_ctor_new(name, input_arity, output_arity);
    def_t *def = def_node_ctor(node_ctor);

    size_t index = 0;

    token_t *input_token = list_first(input_token_list);
    while (input_token) {
        def->node_ctor->port_infos[index] =
            port_info_from_name(string_copy(input_token->string));
        input_token = list_next(input_token_list);
        index++;
    }

    token_t *output_token = list_first(output_token_list);
    while (output_token) {
        def->node_ctor->port_infos[index] =
            port_info_from_name(string_copy(output_token->string));
        output_token = list_next(output_token_list);
        index++;
    }

    mod_define(mod, def);
    return;
}

void
x_define_node(worker_t *worker) {
    token_t *head_token = list_shift(worker->token_list);
    char *name = head_token->string;

    check_name_not_defined(worker, name, head_token);

    list_t *input_token_list = list_new_with((destroy_fn_t *) token_destroy);
    list_t *output_token_list = list_new_with((destroy_fn_t *) token_destroy);

    bool output_flag = false;
    while (true) {
        assert(!list_is_empty(worker->token_list));

        token_t *token = list_shift(worker->token_list);
        if (string_equal(token->string, "end")) {
            token_destroy(&token);
            break;
        }

        if (string_equal(token->string, "--")) {
            output_flag = true;
            token_destroy(&token);
            continue;
        }

        if (output_flag) {
            list_push(output_token_list, token);
        } else {
            list_push(input_token_list, token);
        }
    }

    define_node(worker->mod, name, input_token_list, output_token_list);

    token_destroy(&head_token);
    list_destroy(&input_token_list);
    list_destroy(&output_token_list);
}

void
x_define_rule(worker_t *worker) {
    token_t *first_token = list_shift(worker->token_list);
    token_t *second_token = list_shift(worker->token_list);

    char *first_name = first_token->string;
    char *second_name = second_token->string;

    check_node_name_defined(worker, first_name, first_token);
    check_node_name_defined(worker, second_name, second_token);

    function_t *function = compile_function(worker);
    mod_define_rule(worker->mod, first_name, second_name, function);

    token_destroy(&first_token);
    token_destroy(&second_token);
}
