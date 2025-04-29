#include "index.h"

static int run(commander_t *commander);

void
cmd_default_version(commander_t *commander) {
    command_t *command = command_new("version");
    command->description = "print version";
    command->run = run;
    commander_add(commander, command);
}

int
run(commander_t *commander) {
    printf("%s\n", commander->version);

    return 0;
}
