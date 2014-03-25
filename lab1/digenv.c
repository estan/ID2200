#include "pipe.h"

#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    /* Command argument vectors. */
    char *more_argv[]     = { "more", NULL };
    char *less_argv[]     = { "less", NULL };
    char *pager_argv[]    = { getenv("PAGER"), NULL };
    char *sort_argv[]     = { "sort", NULL };
    char *printenv_argv[] = { "printenv", NULL };
    char **grep_argv      = argv;
    grep_argv[0]          = "grep";

    /* Construct pipeline. */
    
    /*                     file             argv           err  next    fallback */
    command_t more     = { "more",          more_argv,     0,   NULL,   NULL};
    command_t less     = { "less",          less_argv,     0,   NULL,   &more };
    command_t pager    = { getenv("PAGER"), pager_argv,    0,   NULL,   &less };
    command_t sort     = { "sort",          sort_argv,     0,   &pager, NULL };
    command_t grep     = { "grep",          grep_argv,     0,   &sort,  NULL };
    command_t printenv = { "printenv",      printenv_argv, 0,   &sort,  NULL };

    if (argc > 1) {
        /* Arguments given: Run grep as well. */
        printenv.next = &grep;
    }

    /* Run pipeline. */
    run_pipeline(&printenv, STDIN_FILENO);

    return 0;
}

