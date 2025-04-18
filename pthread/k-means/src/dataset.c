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

Dataframe loadRice(const char *filename)
{
    const int MAX_ROWS = 3809;
    const int MAX_COLUMNS = 7;
    const int NUM_FEATURES = 6;
    const int ARFF_COMMENTS_TO_IGNORE = 16;

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error while opening the file");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    for(int i = 0; i < ARFF_COMMENTS_TO_IGNORE; i++) {
        if (fgets(buffer, sizeof(buffer), file) == NULL)
        {
            perror("Error while reading the header");
            fclose(file);
        }
    }

    float **matrix = malloc(MAX_ROWS * sizeof(float *));
    char **features = malloc(NUM_FEATURES * sizeof(char *));

    features[0] = "PerimeterReal";
    features[1] = "MajorAxisLengthReal";
    features[2] = "MinorAxisLengthReal";
    features[3] = "EccentricityReal";
    features[4] = "ConvexArea";
    features[5] = "ExtentReal";

    int row = 0;
    while (row < MAX_ROWS && !feof(file))
    {
        float f1, f2, f3, f4, f5, f6;
        char *label = malloc(50 * sizeof(char));

        int result = fscanf(
            file, 
            "%f,%f,%f,%f,%f,%f,%49[^\n]\n", 
            &f1, &f2, &f3, &f4, &f5, &f6, label
        );

        matrix[row] = malloc((NUM_FEATURES) * sizeof(float));

        matrix[row][0] = f1;
        matrix[row][1] = f2;
        matrix[row][2] = f3;
        matrix[row][3] = f4;
        matrix[row][4] = f5;
        matrix[row][5] = f6;
        row++;
    }

    log_debug("Loaded %d rows", row);

    fclose(file);

    Dataframe df = {
        "rice",
        matrix,
        features,
        MAX_ROWS,
        MAX_COLUMNS,
        NUM_FEATURES,
        0,
        NUM_FEATURES
    };
    return df;
}

Dataframe loadDataset(const char *datasetName)
{
    if (strcmp(datasetName, "iris") == 0) {
        log_debug("Loading Iris dataset...");

        return loadIris("Iris.csv");
    } else if (strcmp(datasetName, "rice") == 0) {
        log_debug("Loading Rice dataset...");

        return loadRice("data/rice/Rice_Cammeo_Osmancik.arff");
    } else {
        log_error("Unknown dataset: %s\n", datasetName);
        exit(EXIT_FAILURE);
    }
}
