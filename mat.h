#ifndef  __MAT_H__
#define  __MAT_H__

#include <math.h>

double _std(int64_t** &mat, int d1, int d2, double mean)
{
    double st;

    for(int i = 0; i < d1; i++)
        for(int j = 0; j < d2; j++)
            st += (mat[i][j] - mean) * (mat[i][j] - mean);

    st /= (d1 * d2 - 1);
    return sqrt(st);
}

double _mean(int64_t** &mat, int d1, int d2)
{
    double sum = 0;
    for(int i = 0; i < d1; i++)
        for(int j = 0; j < d2; j++)
            sum += mat[i][j];

    sum /= (d1 * d2);
    return sum;
}

#endif //__MAT_H__
