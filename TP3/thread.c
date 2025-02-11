#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define SIZE (int)1e8  // 100 millions d'entiers
#define NUM_THREADS 4  // Nombre de threads utilisés

int tab[SIZE];
int global_min, global_max;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Structure pour stocker les limites du thread
typedef struct {
    int start;
    int end;
} ThreadData;

void fill_array() {
    for (int i = 0; i < SIZE; i++) {
        tab[i] = rand() % 1000000;
    }
}

void *find_min_max_thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int local_min = tab[data->start];
    int local_max = tab[data->start];

    for (int i = data->start + 1; i < data->end; i++) {
        if (tab[i] < local_min) local_min = tab[i];
        if (tab[i] > local_max) local_max = tab[i];
    }

    // Protection par mutex
    pthread_mutex_lock(&mutex);
    if (local_min < global_min) global_min = local_min;
    if (local_max > global_max) global_max = local_max;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    struct timeval start, end;
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    global_min = tab[0];
    global_max = tab[0];

    printf("Initialisation du tableau...\n");
    fill_array();

    printf("Recherche du min et max avec %d threads...\n", NUM_THREADS);
    gettimeofday(&start, NULL);

    // Création des threads
    int segment = SIZE / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start = i * segment;
        thread_data[i].end = (i == NUM_THREADS - 1) ? SIZE : (i + 1) * segment;
        pthread_create(&threads[i], NULL, find_min_max_thread, &thread_data[i]);
    }

    // Attente des threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    printf("Min: %d, Max: %d\n", global_min, global_max);
    printf("Temps d'exécution: %.6f secondes\n", elapsed);

    return 0;
}