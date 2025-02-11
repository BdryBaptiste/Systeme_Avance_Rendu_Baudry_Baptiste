/**
 * @file history.h
 * @brief Fonctions de gestion de l'historique
 *
 * Déclare les fonctions pour gérer l'historique des commandes.
 */
#ifndef HISTORY_H
#define HISTORY_H

// Gestion de l'historique
void add_to_history(const char *command);
void load_history();

#endif