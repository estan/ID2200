#include "pipe.h"

#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char *pager_env = getenv("PAGER");

    /* Create pipeline    file        argv                        err  next    fallback */
    command_t more     = {"more",     (char *[]){"more", NULL},     0, NULL,   NULL};
    command_t less     = {"less",     (char *[]){"less", NULL},     0, NULL,   &more};
    command_t pager    = {pager_env,  (char *[]){pager_env, NULL},  0, NULL,   &less};
    command_t sort     = {"sort",     (char *[]){"sort", NULL},     0, &pager, NULL};
    command_t grep     = {"grep",     argv,                         0, &sort,  NULL};
    command_t printenv = {"printenv", (char *[]){"printenv", NULL}, 0, &sort,  NULL};

    argv[0] = "grep"; /* Since we reuse argv for grep */

    if (argc > 1) {
        /* Arguments given: Run grep as well */
        printenv.next = &grep;
    }

    /* Run pipeline */
    run_pipeline(&printenv, STDIN_FILENO);

    return 0;
}
