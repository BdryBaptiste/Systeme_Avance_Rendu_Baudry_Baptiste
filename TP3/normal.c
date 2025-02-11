#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define SIZE (int)1e8  // 100 millions d'entiers
int tab[SIZE];

void fill_array() {
    for (int i = 0; i < SIZE; i++) {
        tab[i] = rand() % 1000000;
    }
}

void find_min_max(int *min, int *max) {
    *min = tab[0];
    *max = tab[0];

    for (int i = 1; i < SIZE; i++) {
        if (tab[i] < *min) *min = tab[i];
        if (tab[i] > *max) *max = tab[i];
    }
}

int main() {
    struct timeval start, end;
    int min, max;

    printf("Initialisation du tableau...\n");
    fill_array();

    printf("Recherche du min et max...\n");
    gettimeofday(&start, NULL);
    find_min_max(&min, &max);
    gettimeofday(&end, NULL);

    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    printf("Min: %d, Max: %d\n", min, max);
    printf("Temps d'exécution: %.6f secondes\n", elapsed);

    return 0;
}