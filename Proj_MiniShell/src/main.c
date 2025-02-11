/**
 * @file main.c
 * @brief Point d'entrée du MiniShell
 *
 * Contient la fonction principale qui initialise le shell.
 */
#include "shell.h"
#include "execute.h"
#include "history.h"
#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024

int main(int argc, char *argv[]) {
    printf("Bienvenue dans MiniShell ! Tapez 'exit' pour quitter.\n");

    //$PATH is correctly set
    setenv("PATH", "/usr/bin:/bin:/usr/local/bin", 1);

    load_history();

    //Check batch mode (`./my_shell -c "command"`)
    if (argc > 1 && strcmp(argv[1], "-c") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Erreur: -c nécessite une commande.\n");
            return 1;
        }

        // Combine all arguments after -c into a single command string
        char command[MAX_INPUT_SIZE] = "";
        for (int i = 2; i < argc; i++) {
            strcat(command, argv[i]);
            if (i < argc - 1) strcat(command, " ");
        }

        printf("🔹 Exécution en mode batch: %s\n", command);
        add_to_history(command);

        // Detect if command is pipe
        if (strchr(command, '|')) {
            execute_piped_commands(command);
        } else {
            execute_logical_commands(command);
        }

        return 0;
    }

    //Start shell
    shell_loop();

    return 0;
}