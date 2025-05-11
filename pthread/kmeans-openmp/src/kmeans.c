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
#include <omp.h>
#include <string.h>

#include "../include/log.h"
#include "../include/helper.h"
#include "../include/experiments.h"

double **initCentroids(Dataframe *df, int k, int expNumber) {
    // Initialize centroids by randomly selecting k data points from the dataset
    srand(time(NULL) + expNumber);

    log_debug("Initializing centroids randomly...");
    double **centroids = malloc(k * sizeof(double *));

    for (int i = 0; i < k; i++)
    {
        int random_index = rand() % df->maxRows;
        centroids[i] = malloc(df->numFeatures * sizeof(double));
        for (int j = 0; j < df->numFeatures; j++)
        {
            centroids[i][j] = df->data[random_index][j];
        }
    }

    log_debug("Centroids initialized!");

    return centroids;
}

int *initAssignments(Dataframe *df, double **centroids, int k) {
    // assign each data point to the nearest centroid
    log_debug("Initializing assignments...");
    int *assignments = malloc(df->maxRows * sizeof(int));

    // k has fixed size, features too, the distance calculation is uniform
    // since there's no imbalance we're using static
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < df->maxRows; i++)
    {
        double minDistance = INFINITY;
        int closestCentroid = -1;

        // TODO: check how performance behaves without simd
        // k is small, not sure if it's worth it to paralell
        for (int j = 0; j < k; j++)
        {
            double sum = 0.0f;
            #pragma omp simd reduction(+:sum)
            for (int l = 0; l < df->numFeatures; l++)
            {
                double diff = df->data[i][l] - centroids[j][l];
                sum += diff * diff;
            }

            double distance = sqrt(sum);

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

// TODO: avoid reallocating the memory
void updateCentroids(Dataframe *df, double **centroids, int *assignments, int k) {
    log_debug("Updating centroids...");

    // allocate the newCentroids
    double **newCentroids = malloc(k * sizeof(double *));
    int *counts = calloc(k, sizeof(int));
    for (int i = 0; i < k; i++) {
        newCentroids[i] = calloc(df->numFeatures, sizeof(double));
    }

    // each thread will work with a different memory to work with
    // this section allocates them
    int num_threads = omp_get_max_threads();
    double ***thread_sums = malloc(num_threads * sizeof(double **));
    int **thread_counts = malloc(num_threads * sizeof(int *));

    for (int t = 0; t < num_threads; t++) {
        thread_counts[t] = calloc(k, sizeof(int));
        thread_sums[t] = malloc(k * sizeof(double *));
        for (int i = 0; i < k; i++) {
            thread_sums[t][i] = calloc(df->numFeatures, sizeof(double));
        }
    }

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < df->maxRows; i++) {
        int tid = omp_get_thread_num();
        int cluster = assignments[i];
        thread_counts[tid][cluster]++;

        for (int j = 0; j < df->numFeatures; j++) {
            thread_sums[tid][cluster][j] += df->data[i][j];
        }
    }

    // join the parallelized thread work
    for (int t = 0; t < num_threads; t++) {
        for (int i = 0; i < k; i++) {
            counts[i] += thread_counts[t][i];
            for (int j = 0; j < df->numFeatures; j++) {
                newCentroids[i][j] += thread_sums[t][i][j];
            }
        }
    }

    for (int i = 0; i < k; i++) {
        if (counts[i] > 0) {
            for (int j = 0; j < df->numFeatures; j++) {
                centroids[i][j] = newCentroids[i][j] / counts[i];
            }
        }
    }

    for (int t = 0; t < num_threads; t++) {
        for (int i = 0; i < k; i++) {
            free(thread_sums[t][i]);
        }
        free(thread_sums[t]);
        free(thread_counts[t]);
    }
    free(thread_sums);
    free(thread_counts);

    for (int i = 0; i < k; i++) {
        free(newCentroids[i]);
    }
    free(newCentroids);
    free(counts);

    log_debug("Centroids updated!");
}


int hasConverged(
    double **currentCentroids,
    double **prevCentroids,
    int k,
    int numFeatures,
    double threshold
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

void kmeans(Dataframe *df, Experiment *exp, int k, int maxIter, int expNumber, int debug) {
    const double CONVERGENCE_THRESHOLD = 1e-6;

    double start, end;
    double wall_time_used;

    start = omp_get_wtime();

    log_debug("Running k-means with k=%d and maxIter=%d...", k, maxIter);

    double **centroids = initCentroids(df, k, expNumber);

    // TODO: separate function to allocate memory for prevCentroids
    double **prevCentroids = malloc(k * sizeof(double *));
    for (int i = 0; i < k; i++) {
        prevCentroids[i] = malloc(df->numFeatures * sizeof(double));
    }

    int *assignments = NULL;
    int iteration = 0;

    while(maxIter > 0)
    {
        assignments = initAssignments(df, centroids, k);

        if(debug) {
            saveIterationData(centroids, assignments, df, k, iteration, expNumber);
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
    }

    end = omp_get_wtime();
    wall_time_used = end - start;

    exp->convergenceIteration = iteration;
    exp->executionTime = wall_time_used;
    exp->number = expNumber;

    for (int i = 0; i < k; i++) {
        free(centroids[i]);
    }
    free(centroids);
    free(assignments);

    log_debug("K-means completed!");
}
