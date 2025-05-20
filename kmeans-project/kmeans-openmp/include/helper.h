#ifndef HELPER_H
#define HELPER_H

typedef struct {
    char *name;
    double **data;
    char **features; // list of features
    int maxRows;
    int maxColumns;
    int numFeatures;
    int startColumn;
    int endColumn;
} Dataframe;

typedef struct {
    int number;
    double executionTime;
    int convergenceIteration;
} Experiment;

// Removed so vectorize with simd
// double euclideanDistance(double *point1, double *point2, int numFeatures);

#endif
