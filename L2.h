#ifndef  __L2_H__
#define  __L2_H__

#include <vector>
#include <stdlib.h>
#include <math.h>

#define sqr(x) (x*x)

/* Euclidean distance function. REQUIRES len1 == len2
 * @input:  list1, the first list
 *          list2, the second list
 * @return: integer representing L2 distance between vectors
 */
int64_t L2(const std::vector<int64_t> &list_a, const std::vector<int64_t> &list_b)
{
    // Local variables
    int len1 = list_a.size(), len2 = list_b.size();
    if(len1 != len2){
        fprintf(stderr, "Vector length mismatch. Aborting.\n");
        exit(1);
    }

    int64_t sum = 0;
    for(int i = 0; i < len1; i++)
        sum += (sqr(list_a[i]) - sqr(list_b[i]));
    sum = (int64_t)sqrt(sum);
    return sum;
}

#endif //__L2_H__
