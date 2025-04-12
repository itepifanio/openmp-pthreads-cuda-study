#ifndef HELPER_H
#define HELPER_H

typedef struct {
    float **data;
    int maxRows;
    int maxColumns;
    int numFeatures;
    int startColumn;
    int endColumn;
} Dataframe;

int isNonNegativeInteger(const char *str);
float euclideanDistance(float *point1, float *point2, int numFeatures);

#endif
