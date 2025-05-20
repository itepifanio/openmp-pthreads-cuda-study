#include <math.h>
#include <ctype.h>


double euclideanDistance(double *point1, double *point2, int numFeatures)
{
    double sum = 0.0;
    for (int i = 0; i < numFeatures; i++)
    {
        double diff = point1[i] - point2[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}
