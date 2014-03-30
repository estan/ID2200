#include "pipe.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/**
 * Turns the file descriptor new into a copy of old.
 *
 * If the redirection succeeds, old will be closed. If old and new is the same file
 * descriptor, this function is a no-op. If redirection fails, the function prints
 * and error and exits the process with EXIT_FAILURE.
 */
void copy_fd(int old, int new) {
    if (new != old) {
        if (dup2(old, new) == -1) {
            perror("dup2");
            _exit(EXIT_FAILURE);
        } else if (close(old)) {
            perror("close");
            _exit(EXIT_FAILURE);
        }
    }
}

/**
 * Invokes a command.
 *
 * This function will try to run the given command using execvp(). If it fails,
 * the fallback command is tried instead. If all fallbacks fails, the function
 * prints an error to stderr and exits the process with EXIT_FAILURE.
 *
 * Commands with a NULL file field are ignored.
 */
void invoke(command_t *command) {
    /* Start with the given command, then successively try each fallback. */
    command_t *current = command;
    while (current) {
        if (current->file) {
            execvp(current->file, current->argv);
            current->err = errno;
        }
        current = current->fallback;
    }

    /* All fallbacks failed. Print error and exit. */
    fprintf(stderr, "Command failed, tried:\n");
    current = command;
    while (current) {
        if (current->file) {
            fprintf(stderr, "  %s: %s\n", current->file, strerror(current->err));
        }
        current = current->fallback;
    }
    exit(EXIT_FAILURE);
}

/**
 * Runs a pipeline starting with the given command and input file descriptor.
 *
 * This function will construct a new pipe for communication between the given
 * command and the next, fork a new child process in which the command is executed,
 * then call itself recursively to set up the rest of the pipeline. Input for
 * the pipeline is taken from the given file descriptor. If at some point an error
 * is encountered, an error message is printed and the recursion stops.
 */
void run_pipeline(command_t *command, int in) {
    int fd[2];
    pid_t pid;
    int status;

    if (command->next == NULL) {
        /* Last command, stop forking. */
        copy_fd(in, STDIN_FILENO);
        invoke(command);
    } else {
        /* Create pipe. */
        if (pipe(fd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        /* Fork a child process. */
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            /* Running in child. */
            if (close(fd[0])) {
                perror("close");
                _exit(EXIT_FAILURE);
            }
            copy_fd(in, STDIN_FILENO);
            copy_fd(fd[1], STDOUT_FILENO);
            invoke(command);
        } else {
            /* Running in parent. */
            if (close(fd[1])) {
                perror("close");
                exit(EXIT_FAILURE);
            }
            if (close(in)) {
                perror("close");
                exit(EXIT_FAILURE);
            }
            wait(&status);
            if (!WEXITSTATUS(status)) {
                /* Child exited normally, run rest of pipeline */
                run_pipeline(command->next, fd[0]);
            }
        }
    }
}

