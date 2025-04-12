#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "../include/log.h"
#include "../include/helper.h"

float **initCentroids(Dataframe *df, int k) {
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
    log_debug("Centroids initialized:");

    for (int i = 0; i < k; i++)
    {
        log_debug("Centroid %d: %f, %f, %f, %f", i, centroids[i][0], centroids[i][1], centroids[i][2], centroids[i][3]);
    }
    log_debug("Centroids initialized!");
}

void kmeans(Dataframe *df, int k, int maxIter) {
    log_debug("Running k-means with k=%d and maxIter=%d...", k, maxIter);

    float **centroids = malloc(k * sizeof(float *));

    log_debug("Starting k-means iterations...");
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

        log_debug("Data point %d assigned to centroid %d", i, closestCentroid);
    }

    log_debug("K-means completed!");
}
