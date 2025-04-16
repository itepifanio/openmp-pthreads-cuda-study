#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../include/log.h"
#include "../include/helper.h"
#include "../include/kmeans.h"
#include "../include/dataset.h"

int main(int argc, char *argv[])
{
    int debug = 0; // debug off

    if (argc < 4 || argc > 6)
    {
        fprintf(
            stderr,
            "Usage: %s <dataset> <clusters> <max_iterations> [debug]\n",
            argv[0]
        );
        fprintf(stderr, "  clusters: non-negative integer\n");
        fprintf(stderr, "  max_iterations: non-negative integer\n");
        fprintf(stderr, "  dataset: filename or 'all'\n");
        fprintf(stderr, "  debug: 0 (off) or 1 (on), default is 0\n");
        return 1;
    }

    char *dataset = argv[1];
    int k = atoi(argv[2]); // clusters
    int maxIter = atoi(argv[3]);
    if (argc >= 5) {
        debug = atoi(argv[4]);
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

    log_info("loading dataset...");
    Dataframe df = loadDataset(dataset);
    log_info("Dataset loaded!");

    log_info("Running k-means...");
    kmeans(&df, k, maxIter, debug);
    log_info("k-means finished!");

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
