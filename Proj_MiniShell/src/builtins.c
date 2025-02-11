/**
 * @file builtins.c
 * @brief Implémentation des commandes internes
 *
 * Gère les commandes internes telles que cd, pwd et exit.
 */
#include "builtins.h"
#include "shell.h"
#include "history.h"

int execute_builtin(char **args) {
    if (args[0] == NULL) return 1;

    if (strcmp(args[0], "cd") == 0) return builtin_cd(args);
    if (strcmp(args[0], "pwd") == 0) return builtin_pwd(args);
    if (strcmp(args[0], "exit") == 0) return builtin_exit(args);
    if (strcmp(args[0], "echo") == 0) return builtin_echo(args);
    if (strcmp(args[0], "export") == 0) return builtin_export(args);
    if (strcmp(args[0], "alias") == 0) return builtin_alias(args);
    if (strcmp(args[0], "history") == 0) return builtin_history(args);

    return 0;
}

int builtin_history(char **args) {
    FILE *file = fopen(HISTORY_FILE, "r");
    if (!file) {
        perror("history");
        return 1;
    }

    char line[MAX_INPUT_SIZE];
    int index = 1;
    while (fgets(line, sizeof(line), file)) {
        printf("%d %s", index++, line);
    }
    fclose(file);
    return 1;
}


int builtin_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "cd: argument manquant\n");
    } else if (chdir(args[1]) != 0) {
        perror("cd");
    }
    return 1;
}

int builtin_pwd(char **args) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("pwd");
    }
    return 1;
}

int builtin_exit(char **args) {
    printf("Sortie du shell...\n");
    exit(0);
}

int builtin_echo(char **args) {
    for (int i = 1; args[i] != NULL; i++) {
        if (args[i][0] == '$') {
            char *var_name = args[i] + 1; // Supprime le '$'
            char *value = getenv(var_name);

            if (value) {
                printf("%s ", value);
            } else {
                printf("%s ", args[i]);  // Afficher tel quel si variable inconnue
            }
        } else {
            printf("%s ", args[i]);
        }
    }
    printf("\n");
    return 1;
}