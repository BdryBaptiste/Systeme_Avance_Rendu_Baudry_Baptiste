/**
 * @file shell.h
 * @brief Fonctions du shell MiniShell
 *
 * En-tête pour les prototypes de fonctions shell.
 */
#ifndef SHELL_H
#define SHELL_H

#include "typedef.h"

void shell_loop();
void add_to_history(const char *command);
int execute_builtin(char **args);

#endif
