/*
Algorithm K-Means Clustering:

1. Initialize centroids
   - Randomly select k data points from the dataset as initial centroids.

2. Repeat until convergence:
   a. Assignment step:
      - For each data point in the dataset:
        i.  Calculate the distance between the data point and each centroid.
        ii. Assign the data point to the nearest centroid.

   b. Update step:
      - For each centroid:
        i.  Calculate the new centroid by taking the mean of all data points assigned to it.

3. Convergence criteria:
   - Check if the centroids have stopped moving (i.e., the changes in centroid positions are below a certain threshold).
   - If centroids have converged, terminate the algorithm.
   - If not, repeat steps 2a and 2b.

End Algorithm
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "../include/log.h"

#define MAX_ROWS 150
#define MAX_COLUMNS 6
#define NUM_FEATURES 4
#define DEBUG 1

int isNonNegativeInteger(const char *str)
{
    while (*str)
    {
        if (!isdigit(*str))
        {
            return 0;
        }
        str++;
    }
    return 1;
}

float **loadIris(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Erro ao abrir o arquivo");
    }

    char header[256]; // ignora head
    if (fgets(header, sizeof(header), file) == NULL)
    {
        perror("Erro ao ler o cabeçalho do arquivo");
        fclose(file);
    }

    float **matrix = malloc(MAX_ROWS * sizeof(float *));

    int row = 0;
    while (row < MAX_ROWS && !feof(file))
    {
        int id;
        float f1, f2, f3, f4;
        char *label = malloc(50 * sizeof(char)); // preciso alocar memória pro char?

        int result = fscanf(file, "%d,%f,%f,%f,%f,%49[^\n]\n", &id, &f1, &f2, &f3, &f4, label);

        matrix[row] = malloc((NUM_FEATURES) * sizeof(float));

        matrix[row][0] = f1;
        matrix[row][1] = f2;
        matrix[row][2] = f3;
        matrix[row][3] = f4;
        row++;
    }

    log_debug("Loaded %d rows", row);

    fclose(file);
    return matrix;
}

float euclideanDistance(float *point1, float *point2, int numFeatures)
{
    float sum = 0.0;
    for (int i = 0; i < numFeatures; i++)
    {
        float diff = point1[i] - point2[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

int main(int argc, char *argv[])
{
    if(DEBUG) {
        log_add_file_handler("kmeans.log", "a", LOG_DEBUG, "file1");
    }

    log_add_stream_handler(DEFAULT, LOG_INFO, "bash2");
    log_set_quiet("root", true);

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <non-neg int> <non-neg int>\n", argv[0]);
        return 1;
    }

    if (!isNonNegativeInteger(argv[1]) || !isNonNegativeInteger(argv[2]))
    {
        fprintf(stderr, "Error: Both arguments must be non-negative integers.\n");
        return 1;
    }

    int k = atoi(argv[1]); // clusters
    int maxIter = atoi(argv[2]);
    log_debug("loading iris dataset...");
    float **data = loadIris("Iris.csv");
    log_debug("Iris dataset loaded!");

    log_debug("Running k-means with k=%d and maxIter=%d...", k, maxIter);

    log_debug("Initializing centroids randomly...");
    srand(time(NULL));
    float **centroids = malloc(k * sizeof(float *));
    for (int i = 0; i < k; i++)
    {
        int random_index = rand() % MAX_ROWS;
        centroids[i] = malloc(4 * sizeof(float));
        for (int j = 0; j < 4; j++)
        {
            centroids[i][j] = data[random_index][j];
        }
    }
    log_debug("Centroids initialized:");
    for (int i = 0; i < k; i++)
    {
        log_debug("Centroid %d: %f, %f, %f, %f", i, centroids[i][0], centroids[i][1], centroids[i][2], centroids[i][3]);
    }
    log_debug("Centroids initialized!");

    log_debug("Starting k-means iterations...");
    for (int i = 0; i < MAX_ROWS; i++)
    {
        float minDistance = INFINITY;
        int closestCentroid = -1;
        for (int j = 0; j < k; j++)
        {
            float distance = euclideanDistance(data[i], centroids[j], NUM_FEATURES);
            if (distance < minDistance)
            {
                minDistance = distance;
                closestCentroid = j;
            }
        }

        log_debug("Data point %d assigned to centroid %d", i, closestCentroid);
    }


    log_debug("K-means completed!");

    log_debug("Freeing memory...");
    for (int i = 0; i < MAX_ROWS; i++)
    {
        if (data[i] != NULL)
        {
            free(data[i]);
        }
    }
    free(data);

    return 0;
}
