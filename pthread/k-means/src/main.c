
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_ROWS 151
#define NUM_FEATURES 6

int is_non_negative_integer(const char *str) {
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

float **load_iris(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
    }

    char header[256]; // ignora head
    if (fgets(header, sizeof(header), file) == NULL) {
        perror("Erro ao ler o cabeçalho do arquivo");
        fclose(file);
    }

    float **matrix = malloc(MAX_ROWS * sizeof(float *));

    int row = 0;
    while (row < MAX_ROWS && !feof(file)) {
        printf("Loading row %d\n", row);
        int id;
        float f1, f2, f3, f4;
        char *label = malloc(50 * sizeof(char)); // preciso alocar memória pro char?

        int result = fscanf(file, "%d,%f,%f,%f,%f,%49[^\n]\n", &id, &f1, &f2, &f3, &f4, label);

        matrix[row] = malloc((NUM_FEATURES - 2) * sizeof(float));

        matrix[row][0] = f1;
        matrix[row][1] = f2;
        matrix[row][2] = f3;
        matrix[row][3] = f4;
        row++;
    }

    fclose(file);
    return matrix;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <non-neg int> <non-neg int>\n", argv[0]);
        return 1;
    }

    if (!is_non_negative_integer(argv[1]) || !is_non_negative_integer(argv[2])) {
        fprintf(stderr, "Error: Both arguments must be non-negative integers.\n");
        return 1;
    }

    int k = atoi(argv[1]); // clusters
    int maxIter = atoi(argv[2]);
    printf("loading iris dataset...\n");
    float **data = load_iris("Iris.csv");
    printf("Iris dataset loaded!\n");


    printf("Data loaded:\n");
    for(int i = 0; i < MAX_ROWS; i++) {
        if (data[i] != NULL) {
            printf("%f,%f,%f,%f\n", data[i][0], data[i][1], data[i][2], data[i][3]);
        }
    }

    printf("Running k-means with k=%d and maxIter=%d...\n", k, maxIter);

    printf("Initializing centroids randomly...\n");
    srand(time(NULL));
    float **centroids = malloc(k * sizeof(float *));
    for (int i = 0; i < k; i++) {
        int random_index = rand() % MAX_ROWS;
        centroids[i] = malloc(4 * sizeof(float));
        for (int j = 0; j < 4; j++) {
            centroids[i][j] = data[random_index][j];
        }
    }
    printf("Centroids initialized:\n");
    for (int i = 0; i < k; i++) {
        printf("Centroid %d: %f, %f, %f, %f\n", i, centroids[i][0], centroids[i][1], centroids[i][2], centroids[i][3]);
    }
    printf("\n");
    for (int iter = 0; iter < maxIter; iter++) {
        printf("Iteration %d\n", iter);

    }
    printf("K-means completed!\n");


    printf("Freeing memory...\n");
    for(int i = 0; i < MAX_ROWS; i++) {
        if (data[i] != NULL) {
            free(data[i]);
        }
    }
    free(data);

    return 0;
}
