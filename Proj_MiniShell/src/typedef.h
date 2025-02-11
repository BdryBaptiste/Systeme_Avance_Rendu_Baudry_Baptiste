/**
 * @file typedef.h
 * @brief Définition des Types
 *
 * Contient les définitions de structures et de types utilisées dans le shell.
 */
#ifndef TYPEDEF_H
#define TYPEDEF_H

#define MAX_ALIASES 50
#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64
#define HISTORY_FILE ".my_shell_history"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

// Structure pour les alias
typedef struct {
    char *name;
    char *value;
} Alias;

extern Alias aliases[MAX_ALIASES];
extern int alias_count;

#endif
