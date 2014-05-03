/**
 * Command data structures and functions declarations.
 *
 * @author Elvis Stansvik <stansvik@kth.se>
 * @author Sam Henriksson <hensa@kth.se>
 */
#ifndef COMMAND_H
#define COMMAND_H

/**
 * Command types.
 */
typedef enum command_type_e {
    FOREGROUND, /**< This is a foreground command. */
    BACKGROUND, /**< This is a background command. */
} command_type_t;

/**
 * Command structure.
 */
typedef struct command_s {
    char *line;          /**< Original command line. */
    char **argv;         /**< Command + arguments. */
    int argc;            /**< Number elements in argv. */
    command_type_t type; /**< Type of command. */
} command_t;

/**
 * Reads a command from the user and returns it.
 *
 * The caller must free the returned command with free_command(). If
 * the entered command was empty, NULL is returned.
 */
command_t *read_command();

/**
 * Frees all memory associated with @a command.
 */
void free_command(command_t *command);

/**
 * Handle the built-in command "cd".
 *
 * Returns 1 if successful, otherwise 0.
 */
int handle_cd(command_t *command);

/**
 * Handle the built-in command "exit".
 *
 * Returns 1 if successful, otherwise 0.
 */
int handle_exit(command_t *command);

#endif // COMMAND_H
