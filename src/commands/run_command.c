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
    char **argv = commander_rest_argv(commander);
    while (*argv) {
        char *src = *argv++;
        char *cwd = getcwd(NULL, 0);
        path_t *path = path_new(cwd);
        path_join(path, src);
        load_mod(path);
    }

    return 0;
}
