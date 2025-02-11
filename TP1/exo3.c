#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>

void print_permissions(mode_t mode) {
    char perms[11] = "----------";
    perms[0] = S_ISDIR(mode) ? 'd' : '-';
    perms[1] = (mode & S_IRUSR) ? 'r' : '-';
    perms[2] = (mode & S_IWUSR) ? 'w' : '-';
    perms[3] = (mode & S_IXUSR) ? 'x' : '-';
    perms[4] = (mode & S_IRGRP) ? 'r' : '-';
    perms[5] = (mode & S_IWGRP) ? 'w' : '-';
    perms[6] = (mode & S_IXGRP) ? 'x' : '-';
    perms[7] = (mode & S_IROTH) ? 'r' : '-';
    perms[8] = (mode & S_IWOTH) ? 'w' : '-';
    perms[9] = (mode & S_IXOTH) ? 'x' : '-';

    printf("%s ", perms);
}

void list_directory(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    struct passwd *pw;
    struct group *gr;
    char time_str[20];

    if ((dir = opendir(path)) == NULL) {
        perror("Erreur lors de l'ouverture du répertoire");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (stat(full_path, &file_stat) == -1) {
            perror("Erreur avec stat");
            continue;
        }

        print_permissions(file_stat.st_mode);

        pw = getpwuid(file_stat.st_uid);
        gr = getgrgid(file_stat.st_gid);
        strftime(time_str, sizeof(time_str), "%d-%m-%Y %H:%M", localtime(&file_stat.st_mtime));

        printf("%s %s %ld %s %s\n",
               pw ? pw->pw_name : "unknown",
               gr ? gr->gr_name : "unknown",
               file_stat.st_size,
               time_str,
               entry->d_name);
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    const char *path = (argc > 1) ? argv[1] : ".";

    printf("Liste des fichiers dans : %s\n", path);
    list_directory(path);

    return 0;
}