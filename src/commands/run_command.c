#include "index.h"

static int run(commander_t *commander);

void
cmd_run(commander_t *runner) {
    command_t *command = make_command("run");
    command->description = "run files";
    command->run = run;
    commander_add(runner, command);
}

int
run(commander_t *commander) {
    char **argv = commander_rest_argv(commander);
    while (*argv) {
        char *arg = *argv++;
        path_t *path = make_path_cwd();
        path_resolve(path, arg);
        load_mod(path);
    }

    return 0;
}
