#ifndef  __MAT_H__
#define  __MAT_H__

#include <math.h>

double _std(int64_t** &mat, int d1, int d2, double mean)
{
    double st;

    for(int i = 0; i < d1; i++)
        for(int j = 0; j < d2; j++)
            st += (mat[i][j] - mean) * (mat[i][j] - mean);

    return sqrt(st);
}

double _mean(int64_t** &mat, int d1, int d2)
{
    double sum = 0;
    double len = d1 * d2;
    for(int i = 0; i < d1; i++)
        for(int j = 0; j < d2; j++)
            sum += (mat[i][j] / len);

    return sum;
}

#endif //__MAT_H__
