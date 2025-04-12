#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../include/log.h"
#include "../include/helper.h"
#include "../include/kmeans.h"

#define MAX_ROWS 150
#define MAX_COLUMNS 6
#define NUM_FEATURES 4
#define DEBUG 1

// TODO: make a more generic csv loader function
Dataframe loadIris(const char *filename)
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
    Dataframe df = {matrix, MAX_ROWS, MAX_COLUMNS, NUM_FEATURES, 1, NUM_FEATURES};
    return df;
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
    Dataframe df = loadIris("Iris.csv");
    log_debug("Iris dataset loaded!");

    kmeans(&df, k, maxIter, DEBUG);

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
