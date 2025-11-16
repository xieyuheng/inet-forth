#include "index.h"

static int run(commander_t *commander);

void
cmd_info(commander_t *commander) {
    command_t *command = make_command("info");
    command->description = "print system info";
    command->run = run;
    commander_add(commander, command);
}

int
run(commander_t *commander) {
    (void) commander;

    printf("page size: %ld bytes\n", sysconf(_SC_PAGE_SIZE));
    printf("number of processors: %ld\n", sysconf(_SC_NPROCESSORS_ONLN));
    printf("size of time_t: %ld bits\n", sizeof(time_t) * 8);

    return 0;
}
