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

# Determine the "distance" between 2 stride freq histograms
def dist(hist_a, hist_b):
    dist = 0
    for i in range(0, min(len(hist_a), len(hist_b))):
        dist += (hist_a[i] - hist_b[i])**2

    dist = math.sqrt(dist)
    return dist

# Clustering done if iterations >= max_iter or centers have converged
def done(old_cent, new_cent, iterations, max_iterations)
    if(iterations >= max_iterations):
        return true
    for i in range(0, len(old_cent)):
        if(old_cent[i] != new_cent[i]):
            return false
    return true

# Assign labels to each histogram based on distance from centers
def getLabels(F, centers):
    labels = []
    for F_i in F:
        min = dist(F_i, centers[0])
        index = 0
        for i in range(1, len(centers):
            d = dist(F_i, centers[i])
            if(d < min):
                min = d
                index = i
        labels.append(index)
    return labels

# TODO: Get new centers based on the average of a cluster's points
# NOTE: If a cluster is empty, have to assign a new random center!
def getCenters(F, labels, num_clusters):
    

# K-means cluster the set of histograms F into num_clust clusters with max_iter iterations
def cluster(F, num_clust, max_iter):
    # Generate "random" centers to start
    cent = []
    old_cent = len(F[i]) * [0]
    for i in range(0, max_clust):
        cent.append(len(F[i]) * [i])

    # Set each interval to be labeled as phase 0
    labels = len(F[i]) * [0]

    # Iterate through clustering until done
    iter = 0
    while(not done(old_cent, cent, iter, max_iter)):
        # Save state
        old_cent = cent
        iter += 1

        # Assign phase labels to each histogram based on which center is closest
        labels = getLabels(F, cent)

        # Get new centers
        cent = getCenters(F, labels, num_clust)

def main():
    # Read in data
    F = []
    with open ('input.txt') as file:
        temp = []
        for line in file:
            temp.append(int(line))
            if(len(temp) >= 100):
                F.append(temp)
                temp = []

    # Define constants
    # size of histrogram x-axis
    k = len(F_k)
    # privacy level
    epsilon = 1
    _lambda = math.sqrt(k) * l2_sensitivity() / epsilon
    
    # Phase clustering
    F_k = cluster(F, 8, 10)

    for k in range(0, len(F)):
        F_k = F[k]
 
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
 
        # TODO: What if probabilities are negative??
 
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
