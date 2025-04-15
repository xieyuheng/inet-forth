#include "index.h"

static int run(commander_t *commander);

void
run_command(commander_t *runner) {
    command_t *command = command_new("run");
    command->description = "run files";
    command->run = run;
    commander_add(runner, command);
}

int
run(commander_t *commander) {
    char **args = commander_rest_argv(commander);
    while (*args) {
        char *arg = *args++;

        if (string_equal(arg, "--single-threaded")) {
            single_threaded_flag = true;
        } else {
            file_t *file = file_open_or_fail(arg, "r");
            const char *code = file_read_string(file);
            fclose(file);
            mod_t *mod = mod_new(arg, code);
            import_prelude(mod);
            node_allocator_t *node_allocator = node_allocator_new();
            worker_t *worker = worker_new(mod, node_allocator);
            execute_all(worker);
            mod_destroy(&mod);
            worker_destroy(&worker);
            node_allocator_destroy(&node_allocator);
        }
    }

    return 0;
}
