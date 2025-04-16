#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "../include/helper.h"
#include "../include/log.h"

Dataframe loadIris(const char *filename)
{
    const int MAX_ROWS = 150;
    const int MAX_COLUMNS = 6;
    const int NUM_FEATURES = 4;

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error while opening the file");
        exit(EXIT_FAILURE);
    }

    char header[256]; // ignora head
    if (fgets(header, sizeof(header), file) == NULL)
    {
        perror("Error while reading the header");
        fclose(file);
    }

    char **features = malloc(NUM_FEATURES * sizeof(char *));
    float **matrix = malloc(MAX_ROWS * sizeof(float *));

    features[0] = "SepalLengthCm";
    features[1] = "SepalWidthCm";
    features[2] = "PetalLengthCm";
    features[3] = "PetalWidthCm";

    int row = 0;
    while (row < MAX_ROWS && !feof(file))
    {
        int id;
        float f1, f2, f3, f4;
        char *label = malloc(50 * sizeof(char)); // preciso alocar memória pro char?

        int result = fscanf(
            file, "%d,%f,%f,%f,%f,%49[^\n]\n", &id, &f1, &f2, &f3, &f4, label
        );

        matrix[row] = malloc((NUM_FEATURES) * sizeof(float));

        matrix[row][0] = f1;
        matrix[row][1] = f2;
        matrix[row][2] = f3;
        matrix[row][3] = f4;
        row++;
    }

    log_debug("Loaded %d rows", row);

    fclose(file);
    Dataframe df = {
        "iris",
        matrix,
        features,
        MAX_ROWS,
        MAX_COLUMNS,
        NUM_FEATURES,
        1,
        NUM_FEATURES
    };
    return df;
}

Dataframe loadDataset(const char *datasetName)
{
    if (strcmp(datasetName, "iris") == 0)
    {
        log_debug("Loading Iris dataset...");
        return loadIris("Iris.csv");
    }
    else
    {
        log_error("Unknown dataset: %s\n", datasetName);
        exit(EXIT_FAILURE);
    }
}
