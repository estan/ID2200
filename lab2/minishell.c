/**
 * A minimalistic shell.
 *
 * @author Elvis Stansvik <stansvik@kth.se>
 * @author Sam Henriksson <hensa@kth.se>
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>

#include <readline/history.h>

#include "command.h"
#include "util.h"

int main(int argc, char *argv[]) {

    /* The shell process itself ignores SIGINT. */
    struct sigaction action;
    action.sa_handler = SIG_IGN;
    if (sigaction(SIGINT, &action, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Read the next command.
        command_t *command = read_command();

        // Check for finished background processes.
        int status;
        pid_t pid;
        while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
            printf("Background process %d finished\n", pid);
        }

        if (!command)
            continue; // Ignore empty commands.

        if (match(command->argv[0], "exit")) {
            // Handle built-in exit command.
            if (handle_exit(command)) {
                free_command(command);
                break;
            }
        } else if (match(command->argv[0], "cd")) {
            // Handle built-in cd command.
            handle_cd(command);
        } else {
            // Fork a child process.
            pid = fork();
            if (pid == -1) {
                perror("fork");
            } else if (pid == 0) {
                // Execute command.
                execvp(command->argv[0], command->argv);
                perror(command->argv[0]);
                exit(EXIT_FAILURE);
            } else {
                if (command->type == FOREGROUND) {
                    printf("Spawned foreground process pid: %d\n", pid);

                    struct timeval t0, t1;

                    // Wait for foreground process.
                    gettimeofday(&t0, NULL);
                    waitpid(pid, &status, 0);
                    gettimeofday(&t1, NULL);

                    printf("Foreground process %d terminated\n", pid);
                    printf("wallclock time: %.3f msec\n", elapsed_ms(&t0, &t1));
                } else {
                    printf("Spawned background process pid: %d\n", pid);
                }
            }
        }

        free_command(command);
    }

    clear_history(); // Clear GNU readline history.

    return EXIT_SUCCESS;
}
