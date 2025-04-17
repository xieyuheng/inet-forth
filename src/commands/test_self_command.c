#include "index.h"

static int run(commander_t *commander);

void
test_self_command(commander_t *commander) {
    command_t *command = command_new("test-self");
    command->description = "run self test";
    command->run = run;
    commander_add(commander, command);
}

int
run(commander_t *commander) {
    (void) commander;

    modules_test();
    node_test();

    return 0;
}
