#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <vector>

#include <cuda.h>
#include <cuda_runtime.h>
#include <driver_functions.h>

#define ABS(x) (x > 0 ? x : -x)
#define MIN(x,y) (x < y ? x : y)

__global void SVF()
{



}

__device int DTW(device_vector<int>& interval_i, device_vector<int>& interval_j)
{
    device_vector<device_vector<int>> dtw(interval_i.size());
    for(int i = 0 ; i < dtw.size(); i++)
    {
        dtw[i] = device_vector<int>(interval_j.size());
    }
    
    for(int i = 0; i < dtw.size(); i++)
    {
        dtw[i][0] = (1 << 31) - 1;
        dtw[0][i] = (1 << 31) - 1;
    }

    dtw[0][0] = 0;
    
    for(i = 1; i < dtw.size(); i++)
    {
        for(j = 1; j < dtw[i].size(); j++)
        {
            cost = ABS(interval_i[i] - interval_j[j]);
            dtw[i][j] = cost + MIN(dtw[i-1][j], MIN(dtw[i][j-1], dtw[i-1][j-1]));
        }
    }

    return dtw[interval_i.size() - 1][interval_j.size() - 1];
}

__global void simMatrix(device_vector<device_vector<int>>& v_trace, device_vector<int>& a_trace, int max_len)
{
    int idx = threadIdx.x + blockIdx.x*blockDim.x;
    int i = idx % max_len, j = idx / max_len;

    a_sim[i][j] = ABS(a_trace[i] - a_trace[j]);

    v_sim[i][j] = DTW(v_trace[i], v_trace[j]);
}

void cudaSVF(device_vector<device_vector<int>>& v_trace, device_vector<int>& a_trace)
{
    int max_len = v_trace.size() > a_trace.size() ? v_trace.size() : a_trace.size();
    int size = max_len * max_len;

    // 1024 threads per block
    dim3 blockDim(1024, 1);
    dim3 gridDim((size + blockDim.x - 1) / blockDim.x);

    simMatrix<<<gridDim, blockDim>>>(v_trace, a_trace, max_len);

    cudaThreadSynchronize();
}
