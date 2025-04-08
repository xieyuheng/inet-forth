#include "index.h"

static int run(commander_t *commander);

void
debug_command(commander_t *runner) {
    command_t *command = command_new("debug");
    command->description = "debug files";
    command->run = run;
    commander_add(runner, command);
}

int
run(commander_t *commander) {
    char **argv = commander_rest_argv(commander);
    argv = commander_rest_argv(commander);
    while (*argv) {
        char *src = *argv++;
        if (string_ends_with(src, ".fth")) {
            file_t *file = file_open_or_fail(src, "r");
            const char *code = file_read_string(file);
            fclose(file);
            core_debug_flag = true;
            mod_t *mod = mod_new(src, code);
            import_prelude(mod);
            node_allocator_t *node_allocator = node_allocator_new(
                NODE_COUNT, NODE_ALLOCATOR_CACHE_SIZE);            
            worker_t *worker = worker_new(mod, node_allocator);
            execute_all(worker);
            debug_start(worker);
            mod_destroy(&mod);
            worker_destroy(&worker);
            node_allocator_destroy(&node_allocator);
        } else  {
            fprintf(stderr, "[debug] file name must ends with .lisp, given file name: %s\n", src);
            exit(1);
        }
    }

    return 0;
}
