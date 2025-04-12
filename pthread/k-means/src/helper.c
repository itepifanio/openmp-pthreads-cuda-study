#include <math.h>
#include <ctype.h>

int isNonNegativeInteger(const char *str)
{
    while (*str)
    {
        if (!isdigit(*str))
        {
            return 0;
        }
        str++;
    }
    return 1;
}

float euclideanDistance(float *point1, float *point2, int numFeatures)
{
    float sum = 0.0;
    for (int i = 0; i < numFeatures; i++)
    {
        float diff = point1[i] - point2[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}
