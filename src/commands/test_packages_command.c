#include "index.h"

static int run(commander_t *commander);

void
cmd_test_helpers(commander_t *commander) {
    command_t *command = make_command("test-helpers");
    command->description = "run test for helpers";
    command->run = run;
    commander_add(commander, command);
}

int
run(commander_t *commander) {
    (void) commander;

    test_helpers();

    return 0;
}
