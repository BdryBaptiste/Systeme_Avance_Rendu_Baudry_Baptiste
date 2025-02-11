#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

//cat /tmp/proc-exercise

void affiche(const char *word) {
    printf("Mot affiché : %s\n", word);
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <mot>\n", argv[0]);
        return EXIT_FAILURE;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Erreur fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) { 
        printf("Fils (PID=%d)\n", getpid());

        int fd = open("/tmp/proc-exercise", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("Erreur ouverture fichier");
            return EXIT_FAILURE;
        }

        dup2(fd, STDOUT_FILENO);
        close(fd);

        affiche(argv[1]);

        exit(EXIT_SUCCESS);
    } else {
        printf("Père (PID=%d), attend le fils...\n", getpid());
        wait(NULL);
        printf("That’s All Folks!\n");
    }

    return 0;
}