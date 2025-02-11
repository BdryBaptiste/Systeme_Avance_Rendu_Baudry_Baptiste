/**
 * @file builtins.h
 * @brief Fonctions des commandes internes
 *
 * Déclare les fonctions des commandes internes telles que cd, pwd et exit.
 */
#ifndef BUILTINS_H
#define BUILTINS_H

// Fonctions des commandes internes
int builtin_cd(char **args);
int builtin_pwd(char **args);
int builtin_exit(char **args);
int builtin_echo(char **args);
int builtin_export(char **args);
int builtin_alias(char **args);

#endif