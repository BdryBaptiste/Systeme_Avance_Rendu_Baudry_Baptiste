/**
 * @file env.c
 * @brief Gestion des variables d'environnement
 *
 * Gère les variables d'environnement.
 */
#include "env.h"
#include "shell.h"

int builtin_export(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "export: usage: export VAR=val\n");
        return 1;
    }

    char *var = strtok(args[1], "=");
    char *val = strtok(NULL, "=");

    if (val == NULL) {
        fprintf(stderr, "export: format invalide\n");
        return 1;
    }

    if (setenv(var, val, 1) != 0) {
        perror("export");
        return 1;
    }
    
    return 1;
}