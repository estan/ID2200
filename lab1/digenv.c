#include "pipe.h"

#include <stdlib.h>

int main(int argc, const char **argv) {

    const char *printenv_argv[] = { "printenv", NULL };
    command_t printenv_command = { "printenv", printenv_argv, NULL };

    const char **grep_argv = argv;
    grep_argv[0] = "grep";
    command_t grep_command = { "grep", grep_argv, NULL };

    const char *more_argv[] = { "more", NULL };
    command_t more_command = { "more", more_argv, NULL };

    const char *less_argv[] = { "less", NULL };
    command_t less_command = { "less", less_argv, &more_command };

    const char *pager_argv[] = { getenv("PAGER"), NULL };
    command_t pager_command = { getenv("PAGER"), pager_argv, &less_command };

    command_t pipeline[] = {
        printenv_command,
        grep_command,
        pager_command
    };

    run_pipeline(pipeline, 3, 0);

    return 0;
}

