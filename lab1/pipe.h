/**
 * Pipeline functions / data structures.
 */
#ifndef PIPE_H
#define PIPE_H

/**
 * Represents a command in a pipeline.
 *
 * Commands are linked together to form a pipeline using the next field.
 * A command can have a fallback command specified in the fallback field.
 * The fallback command will be executed if execution of the command
 * fails.
 */
typedef struct command_s {
    const char *file;           /* File to execute. */
    char * const *argv;         /* NULL-terminated array of arguments. */
    int err;                    /* Error from last execution attempt. */
    struct command_s *next;     /* Next command in pipeline. */
    struct command_s *fallback; /* Fallback command. */
} command_t;

void run_pipeline(command_t *command, int in);

#endif /* PIPE_H */
