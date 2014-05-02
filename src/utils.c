#include "utils.h"
#include <math.h>

double sample_mean(double *vals, int n)
{
    int    i   = 0;
    double sum = 0;
    for(;i<n;i++){
        sum += vals[i];
    }
    return (sum/i);
}

double sample_std_dev(double *vals, int n, double sample_mean)
{
    int    i = 0;
    double P = 0;
    for(;i<n;i++){
        P += pow((vals[i] - sample_mean), 2);
    }
    return sqrt(P/(i-1));
}
