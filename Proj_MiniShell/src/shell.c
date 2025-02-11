/**
 * @file shell.c
 * @brief Fonctions principales du shell
 *
 * Implémenter la boucle principale du shell et la gestion des commandes.
 */
#include "shell.h"
#include "execute.h"
#include "history.h"
#include "builtins.h"
#include "alias.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024

void shell_loop() {
    char input[MAX_INPUT_SIZE];

    while (1) {
        printf("my_shell> ");
        fflush(stdout);

        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = 0;  //Remove trailing newline

        //Skip empty input
        if (strlen(input) == 0) continue;

        add_to_history(input);

        //History command execution
        if (input[0] == '!' && isdigit(input[1])) {
            if (execute_history_command(input) == -1) {
                fprintf(stderr, "No such command in history.\n");
            }
            continue;
        }

        //Alias before execution
        char *resolved_input = resolve_alias(input);
        if (resolved_input != input) {
            strcpy(input, resolved_input);
        }

        //Logical operators
        if (strstr(input, "&&") || strstr(input, "||")) {
            execute_logical_commands(input);
        }
        //Here-Document
        else if (strstr(input, "<<")) {
            printf("[DEBUG] Here-document detected.\n");
            execute_here_document(input);
        }
        //Redirections
        else if (strchr(input, '>') || strchr(input, '<')) {
            execute_redirected_command(input);
        }
        //Pipes
        else if (strchr(input, '|')) {
            execute_piped_commands(input);
        }
        //Background Execution
        else if (strchr(input, '&')) {
            execute_single_command(input);
        }
        //Normal single commands
        else {
            execute_single_command(input);
        }

        fflush(stdout);
    }
}