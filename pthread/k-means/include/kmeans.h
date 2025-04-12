#ifndef KMEANS_H
#define KMEANS_H

float **initCentroids(Dataframe *df, int k);
void kmeans(Dataframe *df, int k, int maxIter);

#endif
