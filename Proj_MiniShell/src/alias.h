/**
 * @file alias.h
 * @brief Fonctions de gestion des alias
 *
 * Déclare les fonctions de gestion des alias.
 */
#ifndef ALIAS_H
#define ALIAS_H

#include "typedef.h"

// Gestion des alias
void add_alias(char *name, char *value);
char *resolve_alias(char *command);
int builtin_alias(char **args);

#endif