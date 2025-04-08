#include "index.h"

static int run(commander_t *commander);

void
self_test_command(commander_t *commander) {
    command_t *command = command_new("self-test");
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
