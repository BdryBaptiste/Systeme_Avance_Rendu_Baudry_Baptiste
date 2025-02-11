/**
 * @file history.c
 * @brief Gestion de l'historique des commandes
 *
 * Gère l'ajout des commandes à l'historique et le chargement de l'historique.
 */
#include "typedef.h"
#include "shell.h"

void add_to_history(const char *command) {
    FILE *file = fopen(HISTORY_FILE, "a");
    if (file == NULL) {
        perror("Erreur ouverture historique");
        return;
    }
    fprintf(file, "%s\n", command);
    fclose(file);
}

void load_history() {
    FILE *file = fopen(HISTORY_FILE, "r");
    if (file == NULL) return;

    char line[MAX_INPUT_SIZE];
    printf("Historique des commandes :\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}