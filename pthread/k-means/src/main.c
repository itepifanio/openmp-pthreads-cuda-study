#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../include/log.h"
#include "../include/helper.h"
#include "../include/kmeans.h"
#include "../include/dataset.h"
#include "../include/experiments.h"

int main(int argc, char *argv[])
{
    int debug = 0; // debug off

    if (argc < 5 || argc > 7)
    {
        fprintf(
            stderr,
            "Usage: %s <dataset> <num_exp> <clusters> <max_iterations> [debug]\n",
            argv[0]
        );
        fprintf(stderr, "  dataset (str): dataset to use\n");
        fprintf(stderr, "  num_exp (int+): number of experiments to run\n");
        fprintf(
            stderr, "  clusters (int+): k number of clusters to separate the data\n"
        );
        fprintf(
            stderr, "  max_iterations (int+): Maximum number of iterations\n"
            "if the algorithm does not converge\n"
        );
        fprintf(stderr, "  debug: 0 (off) or 1 (on), default is 0\n");
        return 1;
    }

    char *dataset = argv[1];
    int numExp = atoi(argv[2]); // number of experiments
    int k = atoi(argv[3]); // clusters
    int maxIter = atoi(argv[4]);
    if (argc >= 5) {
        debug = atoi(argv[5]);
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
        log_add_stream_handler(DEFAULT, LOG_INFO, "console");
    }

    Experiment experiments[numExp];

    log_info("loading dataset...");
    Dataframe df = loadDataset(dataset);
    log_info("Dataset loaded!");

    log_info("Running k-means...");
    for(int i = 0; i < numExp; i++){
        log_debug("Running experiment %d...\n", i);
        kmeans(&df, &experiments[i], k, maxIter, i, debug);
    }
    log_info("k-means finished!");

    for(int i = 0; i < numExp; i++ && debug) {
        log_info("Experiment %d took %f", i+1, experiments[i].executionTime);
    }

    if(! debug) {
        saveExperiment(&experiments, numExp, df.name);
    }

    log_debug("Freeing memory...");
    for (int i = 0; i < df.maxRows; i++)
    {
        if (df.data[i] != NULL)
        {
            free(df.data[i]);
        }
    }

    return 0;
}
