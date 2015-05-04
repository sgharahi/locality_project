#! /usr/bin/python

import sys
import numpy
import math

# Dynamic time warping algorithm
def DTW(hist_a, hist_b):
    dtw = np.zeros((len(hist_a), len(hist_b)))

    for i in range(0, len(hist_a)):
        dtw[i][0] = sys.maxsize
    for i in range(0, len(hist_b)):
        dtw[0][i] = sys.maxsize

    dtw[0][0] = 0

    for i in range(1, len(hist_a)):
        for j in range(1, len(hist_b)):
            cost = abs(hist_a[i] - hist_b[j])
            dtw[i][j] = cost + min([dtw[i-1][j], dtw[i][j-1], dtw[i-1][j-1]])

    return dtw[len(hist_a) - 1][len(hist_b) - 1] / (len(hist_a)*len(hist_b))


# TODO - Find out \Delta_2(Q) (L2 sensitivity of Q)
# by running DTW on set of input histograms
def l2_sensitivity():
    return 50000

def main():
    # Read in data
    F_k = []
    with open ('input.txt') as file:
        for line in file:
            F_k.append(int(line))

    # Define constants
    # size of histrogram x-axis
    k = len(F_k)
    # privacy level
    epsilon = 1
    _lambda = math.sqrt(k) * l2_sensitivity() / epsilon
    
    # Calculate LPA(F^k, \lambda)
    output = []
    for f in F_k:
        # Laplace perturbation
        noise = numpy.random.laplace(0, _lambda)
        output.append(f + noise)

    # Generate probablity model
    length = len(output)
    for i in range(0, length):
        output[i] = int(output[i] * 100 / length)
        print output[i]

    # What if probabilities are negative??

    sys.exit(0)

    # Simulation?
    start = trace.pop(0)
    f_addr = start
    #mem_x(start)
    for r_addr in trace:
        while(true):
            rand = int(math.rand()*length)
            stride = 0
            while(rand > output[stride] and stride < length - 1):
                rand -= output[stride]
                stride += 1
            f_addr += stride
    
            # TODO - adjust probabilities

            # Execute memory transaction
            if(f_addr == r_addr):
                mem_x(r_addr)
                break
            else:
                mem_x(f_addr)

main()
