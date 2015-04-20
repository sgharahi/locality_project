#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <vector>

#include <cuda.h>
#include <cuda_runtime.h>
#include <driver_functions.h>

void
CudaRenderer::render() {

    int size = imageWidth * imageHeight;

    // 256 threads per block is a healthy number
    dim3 blockDim(THREADS, 1);
    dim3 gridDim((size + blockDim.x - 1) / blockDim.x);

    if (sceneName == SNOWFLAKES || sceneName == SNOWFLAKES_SINGLE_FRAME)
        kernelRenderCircles<true><<<gridDim, blockDim>>>();
    else
        kernelRenderCircles<false><<<gridDim, blockDim>>>();

    cudaThreadSynchronize();
}

void main()
{
}
