/**
 * @file alias.c
 * @brief Gestion des alias
 *
 * Gestion de la création et de la résolution des alias de commandes.
 */
#include "alias.h"
#include "shell.h"

Alias aliases[MAX_ALIASES];
int alias_count = 0;

void add_alias(char *name, char *value) {
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(aliases[i].name, name) == 0) {
            free(aliases[i].value);
            aliases[i].value = strdup(value);
            printf("Updated alias: %s='%s'\n", name, value);
            return;
        }
    }
    if (alias_count < MAX_ALIASES) {
        aliases[alias_count].name = strdup(name);
        aliases[alias_count].value = strdup(value);
        alias_count++;
        printf("Added alias: %s='%s'\n", name, value);
    } else {
        printf("Alias limit reached!\n");
    }
}

char *resolve_alias(char *command) {
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(command, aliases[i].name) == 0) {
            printf("Resolved alias: %s -> %s\n", command, aliases[i].value);
            return strdup(aliases[i].value);
        }
    }
    return command;
}

int builtin_alias(char **args) {
    if (args[1] == NULL) {
        return 1;
    }

    // Ensure there's an '=' sign
    char *equal_sign = strchr(args[1], '=');
    if (!equal_sign) {
        printf("Alias format incorrect. Use: alias name='command'\n");
        return 1;
    }

    // Extract alias name
    int name_length = equal_sign - args[1]; 
    char name[100];  // Assuming max alias name length is 100
    strncpy(name, args[1], name_length);
    name[name_length] = '\0';

    // Reconstruct alias value from remaining arguments
    char value[512] = "";  // Assuming max alias value length is 512
    strcat(value, equal_sign + 1);  // Add the part after '='

    for (int i = 2; args[i] != NULL; i++) {
        strcat(value, " ");
        strcat(value, args[i]);
    }

    //Remove surrounding quotes (' or ") if they exist
    int len = strlen(value);
    if ((value[0] == '\'' && value[len - 1] == '\'') || (value[0] == '"' && value[len - 1] == '"')) {
        memmove(value, value + 1, len - 2);  // Shift left to remove first char
        value[len - 2] = '\0';  // Remove last char
    }

    // Store alias
    add_alias(name, value);
    printf("Added alias: %s='%s'\n", name, value);
    
    return 1;
}