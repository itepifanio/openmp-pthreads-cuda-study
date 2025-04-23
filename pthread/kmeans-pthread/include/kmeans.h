#ifndef KMEANS_H
#define KMEANS_H

void kmeans(
    Dataframe *df, 
    Experiment *exp, 
    int k, 
    int maxIter, 
    int numExp, 
    int numThreads,
    int debug
);

#endif
