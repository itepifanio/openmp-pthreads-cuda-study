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
#include <time.h>
#include <math.h>

#include "../include/log.h"
#include "../include/helper.h"

float **initCentroids(Dataframe *df, int k) {
    // Initialize centroids by randomly selecting k data points from the dataset
    srand(time(NULL));

    log_debug("Initializing centroids randomly...");
    float **centroids = malloc(k * sizeof(float *));

    for (int i = 0; i < k; i++)
    {
        int random_index = rand() % df->maxRows;
        centroids[i] = malloc(df->numFeatures * sizeof(float));
        for (int j = 0; j < df->numFeatures; j++)
        {
            centroids[i][j] = df->data[random_index][j];
        }
    }

    log_debug("Centroids initialized!");

    return centroids;
}

int *initAssignments(Dataframe *df, float **centroids, int k) {
    // assign each data point to the nearest centroid
    log_debug("Initializing assignments...");
    int *assignments = malloc(df->maxRows * sizeof(int));

    for (int i = 0; i < df->maxRows; i++)
    {
        float minDistance = INFINITY;
        int closestCentroid = -1;
        for (int j = 0; j < k; j++)
        {
            float distance = euclideanDistance(df->data[i], centroids[j], df->numFeatures);
            if (distance < minDistance)
            {
                minDistance = distance;
                closestCentroid = j;
            }
        }

        assignments[i] = closestCentroid;
    }

    return assignments;
}

void updateCentroids(Dataframe *df, float **centroids, int *assignments, int k) {
    log_debug("Updating centroids...");

    float **newCentroids = malloc(k * sizeof(float *));
    int *counts = calloc(k, sizeof(int));

    for (int i = 0; i < k; i++) {
        newCentroids[i] = calloc(df->numFeatures, sizeof(float));
    }

    // Sum up data points assigned to each centroid
    for (int i = 0; i < df->maxRows; i++) {
        int cluster = assignments[i];
        counts[cluster]++;
        for (int j = 0; j < df->numFeatures; j++) {
            newCentroids[cluster][j] += df->data[i][j];
        }
    }

    // Calculate the mean for each centroid
    for (int i = 0; i < k; i++) {
        if (counts[i] > 0) {
            for (int j = 0; j < df->numFeatures; j++) {
                centroids[i][j] = newCentroids[i][j] / counts[i];
            }
        }
    }

    // Free temporary storage
    for (int i = 0; i < k; i++) {
        free(newCentroids[i]);
    }
    free(newCentroids);
    free(counts);

    log_debug("Centroids updated!");
}

void saveIterationData(float **centroids, int *assignments, Dataframe *df, int k, int iteration) {
    // TODO: add which dataframe was used
    char filename[100];
    sprintf(filename, "experiments/iteration_%03d.csv", iteration);

    FILE *file = fopen(filename, "w");
    if (!file) {
        log_error("Failed to open file for iteration data: %s", filename);
        return;
    }

    fprintf(file, "point_id,SepalLengthCm,SepalWidthCm,PetalLengthCm,PetalWidthCm,cluster\n");

    for (int i = 0; i < df->maxRows; i++) {
        fprintf(file, "%d", i);
        for (int j = 0; j < df->numFeatures; j++) {
            fprintf(file, ",%f", df->data[i][j]);
        }
        fprintf(file, ",%d\n", assignments[i]);
    }

    for (int i = 0; i < k; i++) {
        fprintf(file, "c%d", i);
        for (int j = 0; j < df->numFeatures; j++) {
            fprintf(file, ",%f", centroids[i][j]);
        }
        fprintf(file, ",%d\n", i);
    }

    fclose(file);
    log_debug("Saved iteration %d data to %s", iteration, filename);
}

int hasConverged(
    float **currentCentroids,
    float **prevCentroids,
    int k,
    int numFeatures,
    float threshold
) {
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < numFeatures; j++) {
            if (fabs(currentCentroids[i][j] - prevCentroids[i][j]) > threshold) {
                return 0;
            }
        }
    }
    return 1;
}

void kmeans(Dataframe *df, int k, int maxIter, int debug) {
    const float CONVERGENCE_THRESHOLD = 1e-4;

    log_debug("Running k-means with k=%d and maxIter=%d...", k, maxIter);

    float **centroids = initCentroids(df, k);

    // TODO: separate function to allocate memory for prevCentroids
    float **prevCentroids = malloc(k * sizeof(float *));
    for (int i = 0; i < k; i++) {
        prevCentroids[i] = malloc(df->numFeatures * sizeof(float));
    }

    int *assignments = NULL;
    int iteration = 0;

    while(maxIter > 0)
    {
        assignments = initAssignments(df, centroids, k);

        if(debug) {
            saveIterationData(centroids, assignments, df, k, iteration);
        }

        // save previous centroids before updating
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < df->numFeatures; j++) {
                prevCentroids[i][j] = centroids[i][j];
            }
        }
        updateCentroids(df, centroids, assignments, k);

        if (hasConverged(centroids, prevCentroids, k, df->numFeatures, CONVERGENCE_THRESHOLD)) {
            log_debug("Convergence achieved after %d iterations.", iteration + 1);
            break;
        }

        log_debug("Max iterations left: %d", --maxIter);
        iteration++;

        free(assignments);
    }

    for (int i = 0; i < k; i++) {
        free(centroids[i]);
    }
    free(centroids);

    log_debug("K-means completed!");
}
