#include "index.h"

static int run(commander_t *commander);

void
cmd_test_modules(commander_t *commander) {
    command_t *command = command_new("test-modules");
    command->description = "run test for modules";
    command->run = run;
    commander_add(commander, command);
}

int
run(commander_t *commander) {
    (void) commander;

    test_modules();

    return 0;
}
