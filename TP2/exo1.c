#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Erreur de fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        int child_pid = getpid();
        int parent_pid = getppid();
        printf("Fils: PID=%d, PPID=%d\n", child_pid, parent_pid);
        exit(child_pid % 10);
    } else {
        int status;
        wait(&status);
        printf("Père: Le fils (PID=%d) s'est terminé avec le code %d\n", pid, WEXITSTATUS(status));
    }

    return 0;
}
