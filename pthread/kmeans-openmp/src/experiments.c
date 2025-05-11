#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "../include/helper.h"
#include "../include/log.h"

void saveIterationData(
    double **centroids,
    int *assignments,
    Dataframe *df,
    int k,
    int iteration,
    int expNumber
) {
    char filename[100];
    sprintf(
        filename,
        "experiments/%s_experiment_%d_iteration_%03d.csv",
        df->name,
        expNumber,
        iteration
    );

    FILE *file = fopen(filename, "w");
    if (!file) {
        log_error("Failed to open file for iteration data: %s", filename);
        return;
    }

    int totalFeatureLength = 0;
    for (int i = 0; i < df->numFeatures; i++) {
        totalFeatureLength += strlen(df->features[i]) + 1;
    }

    if(totalFeatureLength == 0) {
        log_error("No features found in dataframe");
        fclose(file);
        return;
    }

    char *features = malloc(totalFeatureLength * sizeof(char *));
    features[0] = '\0';
    for(int i = 0; i < df->numFeatures; i++) {
        strcat(features, df->features[i]);
        if (i < df->numFeatures - 1) {
            strcat(features, ",");
        }
    }

    fprintf(file, "point_id,dataset,%s,cluster\n", features);

    for (int i = 0; i < df->maxRows; i++) {
        fprintf(file, "%d", i);
        fprintf(file, ",%s", df->name);
        for (int j = 0; j < df->numFeatures; j++) {
            fprintf(file, ",%f", df->data[i][j]);
        }
        fprintf(file, ",%d\n", assignments[i]);
    }

    for (int i = 0; i < k; i++) {
        fprintf(file, "c%d", i);
        fprintf(file, ",%s", df->name);
        for (int j = 0; j < df->numFeatures; j++) {
            fprintf(file, ",%f", centroids[i][j]);
        }
        fprintf(file, ",%d\n", i);
    }

    log_debug("Saved iteration %d data to %s", iteration, filename);

    fclose(file);
}

void saveExperiment(Experiment *experiments, int numberExperiments, char *dataframe) {
    char filename[100];
    sprintf(
        filename,
        "experiments/%s_experiment_result.csv",
        dataframe
    );

    FILE *file = fopen(filename, "w");
    if (!file) {
        log_error("Failed to open file for iteration data: %s", filename);
        return;
    }

    fprintf(file, "iteration,dataset,time,converged_at\n");
    for(int i = 0; i < numberExperiments; i++) {
        fprintf(
            file,
            "%d,%s,%f,%d\n",
            i,
            dataframe,
            experiments[i].executionTime,
            experiments[i].convergenceIteration
        );
    }
    fclose(file);
}
