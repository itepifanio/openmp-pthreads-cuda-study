#ifndef HELPER_H
#define HELPER_H

typedef struct {
    char *name;
    float **data;
    char **features; // list of features
    int maxRows;
    int maxColumns;
    int numFeatures;
    int startColumn;
    int endColumn;
} Dataframe;

float euclideanDistance(float *point1, float *point2, int numFeatures);

#endif
