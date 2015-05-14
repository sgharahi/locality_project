#ifndef  __DTW_H__
#define  __DTW_H__

#include <vector>
#include <stdlib.h>

#define MIN3(a,b,c) (a < b ? (a < c ? a : c) : (b < c ? b : c))
#define MIN2(a,b) (a < b ? a : b)
#define ABS(x) ((x)<0 ? -(x) : (x))

/* C implementation of normalized Dynamic Time Warping Algorithm
 * @input:  list1, the first list
 *          list2, the second list
 * @return: integer representing distance between vectors
 */
int64_t DTW(const std::vector<int64_t> &list_a, const std::vector<int64_t> &list_b)
{
    // Local variables
    int len1 = list_a.size(), len2 = list_b.size();
    int64_t **dtw;

    // Set up the matrix for initial calculations
    dtw = (int64_t**) malloc(sizeof(int64_t*) * len1);
    for(int i = 0; i < len1; i++)
        dtw[i] = (int64_t*) malloc(sizeof(int64_t) * len2);

    for(int i = 0; i < len1; i++)
        dtw[i][0] = (1 << 31);
    for(int i = 0; i < len2; i++)
        dtw[0][i] = (1 << 31);

    // Loop through the lists
    for(int i = 1; i < len1; i++)
        for(int j = 1; j < len2; j++)
            dtw[i][j] = ABS(list_a[i] - list_b[j]) + MIN3(dtw[i-1][j], dtw[i][j-1], dtw[i-1][j-1]);
    
    int64_t ret = dtw[len1 -1][len2 - 1] / MIN2(len1, len2);
    free(dtw);
    return ret;
}

#endif //__DTW_H__
