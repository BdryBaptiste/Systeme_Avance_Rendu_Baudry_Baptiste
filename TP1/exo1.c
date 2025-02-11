#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int fd, dest_fd;
    ssize_t nByteRead;    
    char charBuffer[1000];

    if (argc != 3) {
        fprintf(stderr, "Usage: %s fichier 1  fichier 2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDONLY);

    if(fd == -1){
        fprintf(stderr, "Erreur lors de l'ouverture du fichier d'origine %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    dest_fd = open(argv[2], O_WRONLY);
    if (dest_fd == -1) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier de destination %s\n", argv[2]);
        close(fd);
        exit(EXIT_FAILURE);
    }

    while ((nByteRead = read(fd, charBuffer, 1000)) > 0) {
        ssize_t nByteWritten;

        nByteWritten = write(dest_fd, charBuffer, nByteRead);
        if (nByteWritten == -1) {
            perror("Erreur lors de l'écriture dans le fichier destination");
            close(fd);
            close(dest_fd);
            exit(EXIT_FAILURE);
        } else if (nByteWritten != nByteRead) {
            fprintf(stderr, "Erreur : nombre d'octets écrits différent du nombre d'octets lus\n");
            close(fd);
            close(dest_fd);
            exit(EXIT_FAILURE);
        }
    }


    close(fd);
    close(dest_fd);
}