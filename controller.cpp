#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/mman.h>
#include "mkl_cblas.h"

inline int sgn(float f) { if(f > 0) return f; else return -f;}

float laplace(float lambda)
{
    float random = (float)rand()/(float)(RAND_MAX);
    random -= 0.5f;
    float lap = lambda * sgn(random) * log(1 - 2 * abs(random));
    return lap;
}

/* 
 * Inputs: File with memtrace stream
 *         Q - sensitivity level
 *         epsilon - privacy level
 *         Stride probibilities for all phases?
 */
int main(int argc, char* argv[])
{
    if(argc < 4)
    {
        fprintf(stderr, "Usage: %s trace Q epsilon\n", argv[0]).
        return 1;
    }

    // Begin reading in file
    const char* trace;
    int fd = open(argv[1], O_RDONLY);
    stat sb;

    fstat(fd, &sb);

    trace = mmap(NULL, sb.st_size, PROT_WRITE, MAP_SHARED, fd, 0);

    if(trace == MAP_FAILED)
    {
        fprintf(stderr, "Error reading from file: %s\n", argv[1]);
        return 1;
    }

    // Stage 1:
    // Phase detection (what phase am I in -> what are my probabilities?)
    // Each phase has known stride probabilities. Have to add noise based on that.
    // Add noise to intervals of 100 transactions
    
    int lambda = 10 /* sqrt(100) */
                 * atoi(argv[2]) /* sensitivty */
                 / atof(argv[3]); /* epsilon */
    
    
    
    // Stage 2
    // Compute noise and new probabilities
    

    // Stage 3
    // Generate new noisy trace and output it


    return 0;
}
