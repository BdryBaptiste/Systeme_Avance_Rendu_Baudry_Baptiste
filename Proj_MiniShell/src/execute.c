/**
 * @file execute.c
 * @brief Fonctions d'exécution des commandes
 *
 * Handles execution of commands including pipes and redirections.
 * Gère les commandes d'exécution
 */
#include "execute.h"
#include "shell.h"
#include "alias.h"
#include "history.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>


int execute_single_command(char *command) {
    char *args[MAX_ARGS];
    int i = 0;
    int background = 0;

    //Alias before execution
    char *resolved_command = resolve_alias(command);
    if (resolved_command != command) {
        strcpy(command, resolved_command);
    }

    if (command[strlen(command) - 1] == '&') {
        background = 1;
        command[strlen(command) - 1] = '\0';
    }

    char *token = strtok(command, " ");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    if (execute_builtin(args)) {
        return 0;  //Return success
    }

    setenv("PATH", "/usr/bin:/bin:/usr/local/bin", 1);

    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        perror("execvp failed");
        exit(127);  //Command not found
    } else if (pid < 0) {
        perror("fork failed");
        return 1;
    } else {
        if (background) {
            printf("[1] Process started: %d\n", pid);
            return 0;
        } else {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                return WEXITSTATUS(status);  //Child process exit status
            }
            return 1;  //Default error if process did not exit normally
        }
    }
}

int execute_history_command(char *input) {
    int index = atoi(input + 1);
    FILE *file = fopen(HISTORY_FILE, "r");
    if (!file) return -1;

    char line[MAX_INPUT_SIZE];
    int count = 1;
    while (fgets(line, sizeof(line), file)) {
        if (count == index) {
            fclose(file);
            line[strcspn(line, "\n")] = 0;
            printf("Executing: %s\n", line);
            execute_logical_commands(line);
            return 0;
        }
        count++;
    }
    fclose(file);
    return -1;
}

void execute_logical_commands(char *input) {
    char *commands[MAX_ARGS];
    int operators[MAX_ARGS];  //1 for `&&`, 0 for `||`
    int cmd_count = 0;

    char *token = strtok(input, " ");
    char *command = NULL;

    while (token != NULL) {

        if (strcmp(token, "&&") == 0) {
            if (command) {
                commands[cmd_count++] = command;
                operators[cmd_count - 1] = 1;  //`&&`
            }
            command = NULL;
        } else if (strcmp(token, "||") == 0) {
            if (command) {
                commands[cmd_count++] = command;
                operators[cmd_count - 1] = 0;  //`||`
            }
            command = NULL;
        } else {
            if (!command) {
                command = strdup(token);  //New command
            } else {
                char *new_command = malloc(strlen(command) + strlen(token) + 2);
                sprintf(new_command, "%s %s", command, token);
                free(command);
                command = new_command;
            }
        }
        token = strtok(NULL, " ");
    }

    if (command) {
        commands[cmd_count++] = command;
    }

    int last_status = execute_single_command(commands[0]);

    for (int i = 1; i < cmd_count; i++) {

        if ((operators[i - 1] == 1 && last_status == 0) ||  //`&&` requires success
            (operators[i - 1] == 0 && last_status != 0)) {  //`||` requires failure

            last_status = execute_single_command(commands[i]);
        }
        free(commands[i]);  //Free allocated memory !!!!!!
    }
}

void execute_piped_commands(char *input) {
    char *commands[2];
    char *token = strtok(input, "|");

    if (!token) return;
    commands[0] = token;
    token = strtok(NULL, "|");
    if (!token) {
        execute_single_command(commands[0]);
        return;
    }
    commands[1] = token;

    while (*commands[0] == ' ') commands[0]++;
    while (*commands[1] == ' ') commands[1]++;

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe failed");
        return;
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);

        execute_single_command(commands[0]);
        exit(EXIT_FAILURE);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        close(pipe_fd[1]);
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);

        execute_single_command(commands[1]);
        exit(EXIT_FAILURE);
    }

    close(pipe_fd[0]);
    close(pipe_fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

void execute_redirected_command(char *input) {
    char *command, *file;
    char *saveptr;

    int redirect_type = -1;
    if (strstr(input, ">>")) {
        redirect_type = O_WRONLY | O_CREAT | O_APPEND;
    } else if (strstr(input, ">")) {
        redirect_type = O_WRONLY | O_CREAT | O_TRUNC;
    } else if (strstr(input, "<")) {
        redirect_type = O_RDONLY;
    }

    command = strtok_r(input, "><", &saveptr);
    file = strtok_r(NULL, "><", &saveptr);

    if (!command || !file) {
        fprintf(stderr, "Error: Invalid redirection syntax.\n");
        return;
    }

    while (*command == ' ') command++;
    while (*file == ' ') file++;

    int fd;
    if (redirect_type == O_RDONLY) {
        fd = open(file, O_RDONLY);
    } else {
        fd = open(file, redirect_type, 0644);
    }

    if (fd == -1) {
        perror("open failed");
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        if (redirect_type == O_RDONLY) {
            dup2(fd, STDIN_FILENO);
        } else {
            dup2(fd, STDOUT_FILENO);
        }
        close(fd);
        execute_single_command(command);
        exit(EXIT_FAILURE);
    }

    close(fd);
    waitpid(pid, NULL, 0);
}

void trim_whitespace(char *str) {
    int start = 0;
    while (isspace((unsigned char)str[start])) start++;  //Skip leading spaces

    int end = strlen(str) - 1;
    while (end > start && isspace((unsigned char)str[end])) end--;  //Trim trailing spaces

    memmove(str, str + start, end - start + 1);
    str[end - start + 1] = '\0';  //Null-terminate the string
}

void execute_here_document(char *input) {
    char *command, *delimiter;
    char *saveptr;

    command = strtok_r(input, "<<", &saveptr);
    delimiter = strtok_r(NULL, "<<", &saveptr);

    if (!command || !delimiter) {
        fprintf(stderr, "Error: Invalid Here-Document syntax.\n");
        return;
    }

    trim_whitespace(command);  // Remove leading/trailing spaces
    trim_whitespace(delimiter);

    if (strlen(command) == 0 || strlen(delimiter) == 0) {
        fprintf(stderr, "Error: Empty command or delimiter in Here-Document.\n");
        return;
    }

    // Create a temporary file for Here-Document input
    char tmpfile[] = "/tmp/heredocXXXXXX";
    int fd = mkstemp(tmpfile);
    if (fd == -1) {
        perror("mkstemp failed");
        return;
    }

    printf("heredoc> ");
    fflush(stdout);

    char buffer[1024];

    // Read input until delimiter is entered
    while (fgets(buffer, sizeof(buffer), stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline

        if (strcmp(buffer, delimiter) == 0) {
            break;
        }

        write(fd, buffer, strlen(buffer));
        write(fd, "\n", 1);
        printf("heredoc> ");
        fflush(stdout);
    }

    close(fd);

    //Prepare command execution
    char *args[MAX_ARGS];
    int i = 0;
    char *token = strtok(command, " ");  //Split command

    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    //Execute command using the Here-Document file as input
    pid_t pid = fork();
    if (pid == 0) {
        int input_fd = open(tmpfile, O_RDONLY);
        if (input_fd == -1) {
            perror("open failed");
            exit(EXIT_FAILURE);
        }
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);

        execvp(args[0], args);
        perror("execlp failed");
        exit(EXIT_FAILURE);
    }

    waitpid(pid, NULL, 0);
    unlink(tmpfile);  //Remove temp file
}