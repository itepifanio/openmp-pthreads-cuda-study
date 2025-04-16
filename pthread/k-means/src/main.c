#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../include/log.h"
#include "../include/helper.h"
#include "../include/kmeans.h"

#define MAX_ROWS 150
#define MAX_COLUMNS 6
#define NUM_FEATURES 4

// TODO: make a more generic csv loader function
Dataframe loadIris(const char *filename)
{
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
    Dataframe df = {matrix, MAX_ROWS, MAX_COLUMNS, NUM_FEATURES, 1, NUM_FEATURES};
    return df;
}

int main(int argc, char *argv[])
{
    int debug = 0; // debug off

    if (argc < 3 || argc > 5)
    {
        fprintf(stderr, "Usage: %s <clusters> <max_iterations> [debug]\n", argv[0]);
        fprintf(stderr, "  clusters: non-negative integer\n");
        fprintf(stderr, "  max_iterations: non-negative integer\n");
        fprintf(stderr, "  debug: 0 (off) or 1 (on), default is 0\n");
        return 1;
    }

    if (argc >= 4) {
        debug = atoi(argv[3]);
        if (debug != 0 && debug != 1) {
            fprintf(stderr, "Debug must be 0 or 1\n");
            return 1;
        }
    }

    // setup logging: if debug is on log to file, otherwise log errors to console
    log_set_quiet("root", true);  // disable logging for root
    if(debug) {
        log_add_file_handler("kmeans.log", "a", LOG_DEBUG, "file1");
        log_add_stream_handler(DEFAULT, LOG_DEBUG, "console");
    } else {
        log_add_stream_handler(DEFAULT, LOG_ERROR, "console");
    }

    int k = atoi(argv[1]); // clusters
    int maxIter = atoi(argv[2]);

    log_debug("loading iris dataset...");
    Dataframe df = loadIris("Iris.csv");
    log_debug("Iris dataset loaded!");

    kmeans(&df, k, maxIter, debug);

    log_debug("Freeing memory...");
    for (int i = 0; i < MAX_ROWS; i++)
    {
        if (df.data[i] != NULL)
        {
            free(df.data[i]);
        }
    }

    return 0;
}
