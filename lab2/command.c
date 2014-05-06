/**
 * Command functions definitions.
 *
 * See command.h for documentation.
 *
 * @author Elvis Stansvik <stansvik@kth.se>
 * @author Sam Henriksson <hensa@kth.se>
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "util.h"
#include "command.h"

static const char *WHITESPACE = " \t\r\n\v\f";

command_t *read_command() {
    command_t *command = malloc(sizeof(command_t));

    /* Read a line of input with GNU readline. */
    command->line = readline("minishell> ");
    if (is_empty(command->line)) {
        free(command);
        return NULL;
    }

    /* Add to GNU readline history. */
    add_history(command->line);

    /* Parse command and arguments. */
    int argv_len = 4;
    command->argv = malloc(sizeof(char *) * argv_len);
    command->argv[0] = strtok(command->line, WHITESPACE);
    command->argc = 1;
    while ((command->argv[command->argc] = strtok(NULL, WHITESPACE))) {
        if (argv_len < command->argc + 2) {
            // Increase size of argv.
            argv_len *= 2;
            command->argv = realloc(command->argv, sizeof(char *) * argv_len);
        }
        ++command->argc;
    }
    command->argv[command->argc] = NULL;

    /* Determine command type. */
    if (command->argc > 1 && match(command->argv[command->argc - 1], "&")) {
        // Trailing '&', so it's a background command.
        command->argv[command->argc - 1] = NULL;
        command->type = BACKGROUND;
        --command->argc;
    } else {
        // It's a foreground command.
        command->type = FOREGROUND;
    }

    return command;
}

void free_command(command_t *command) {
    if (!command)
        return;
    free(command->line);
    free(command->argv);
    free(command);
}

int handle_cd(command_t *command) {
    // Check syntax.
    if (command->argc > 2) {
        fprintf(stderr, "Usage: cd [<dir>]\n");
        return 0;
    }

    // Change directory.
    if (chdir(command->argv[1]) == -1) {
        if (command->argc > 1)
            perror(command->argv[1]);

        // Try HOME instead.
        fprintf(stderr, "cd: Trying HOME...\n");
        const char *home = getenv("HOME");
        if (!home) {
            fprintf(stderr, "cd: HOME is not set!\n");
            return 0;
        } else if (chdir(home) == -1) {
            perror(home);
            return 0;
        } else {
            return 1;
        }
    }

    return 1;
}

int handle_exit(command_t *command) {
    // We just check that the syntax is OK.
    if (command->argc != 1) {
        fprintf(stderr, "Usage: exit\n");
        return 0;
    } else {
        return 1;
    }
}

