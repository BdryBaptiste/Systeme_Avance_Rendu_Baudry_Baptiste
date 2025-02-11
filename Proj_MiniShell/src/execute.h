/**
 * @file execute.h
 * @brief Fonctions d'exécution des commandes
 *
 * Déclare les fonctions pour l'exécution des commandes.
 */
#ifndef EXECUTE_H
#define EXECUTE_H

#include "typedef.h"

// Function prototypes
void execute_logical_commands(char *input);
int execute_single_command(char *command);
void execute_piped_commands(char *input);
void execute_redirected_command(char *input);
void execute_here_document(char *input);

#endif