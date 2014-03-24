/**
 * Pipeline functions
 *
 * TODO: More docs.
 */
#ifndef PIPE_H
#define PIPE_H

typedef struct command_s {
    const char *file;
    const char **argv;
    struct command_s *next;
} command_t;

void run_pipeline(command_t *commands, int n, int first) {
    // TODO: pipe + fork + dup2 + execvp recursively
}

#endif /* PIPE_H */
