#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int fd;
    off_t fileSize;    
    char character; 

    if (argc != 2) {
        fprintf(stderr, "Usage: %s fichier 1 \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDONLY);

    if(fd == -1){
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    fileSize = lseek(fd, 0, SEEK_END);
    if (fileSize == -1) {
        perror("Erreur lors de la détermination de la taille du fichier");
        close(fd);
        exit(EXIT_FAILURE);
    }

    for (off_t i = 1; i <= fileSize; i++) {
        if (lseek(fd, -i, SEEK_END) == -1) {
            perror("Erreur lors du repositionnement du descripteur de fichier");
            close(fd);
            exit(EXIT_FAILURE);
        }

        if (read(fd, &character, 1) != 1) {
            perror("Erreur lors de la lecture du fichier");
            close(fd);
            exit(EXIT_FAILURE);
        }

        if (write(STDOUT_FILENO, &character, 1) != 1) {
            perror("Erreur lors de l'écriture sur la sortie standard");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    close(fd);

    return EXIT_SUCCESS;
}