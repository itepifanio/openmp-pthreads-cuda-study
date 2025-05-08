#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

void saveIterationData(
    float **centroids,
    int *assignments,
    Dataframe *df,
    int k,
    int iteration,
    int expNumber
);

void saveExperiment(
    Experiment *experiments, 
    int numberExperiments, 
    char *dataframe
);

#endif
