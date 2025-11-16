#include "index.h"

static int run(commander_t *commander);

void
cmd_default_help(commander_t *commander) {
    command_t *command = make_command("help");
    command->description = "print help message";
    command->run = run;
    commander_add(commander, command);
}

int
run(commander_t *commander) {
    commander_help(commander);

    return 0;
}
