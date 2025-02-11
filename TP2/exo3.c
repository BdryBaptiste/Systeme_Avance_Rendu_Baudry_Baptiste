#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Erreur pipe");
        return EXIT_FAILURE;
    }

    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("Erreur fork");
        return EXIT_FAILURE;
    }

    if (pid1 == 0) {  // Processus Fils 1 (ps eaux)
        close(pipefd[0]);  // Ferme la lecture
        dup2(pipefd[1], STDOUT_FILENO);  // Redirige stdout vers pipe
        close(pipefd[1]);

        execlp("ps", "ps", "eaux", NULL);
        perror("Erreur exec ps");
        exit(EXIT_FAILURE);
    }

    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("Erreur fork");
        return EXIT_FAILURE;
    }

    if (pid2 == 0) {  // Processus Fils 2 (grep "^root ")
        close(pipefd[1]);  // Ferme l’écriture
        dup2(pipefd[0], STDIN_FILENO);  // Redirige stdin vers pipe
        close(pipefd[0]);

        int devnull = open("/dev/null", O_WRONLY);
        dup2(devnull, STDOUT_FILENO);  // Redirige stdout vers `/dev/null`
        close(devnull);

        execlp("grep", "grep", "^root ", NULL);
        perror("Erreur exec grep");
        exit(EXIT_FAILURE);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    int status;
    waitpid(pid2, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        write(STDOUT_FILENO, "root est connecté\r\n", 18);
    }

    return 0;
}