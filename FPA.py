#! /usr/bin/python

import sys
import numpy
import math
import copy
from sens import sensitivity

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
# by running DTW on set of input histograms. Call sensitivity.py for this
def l2_sensitivity():
    return 50000

# Determine the "distance" between 2 stride freq histograms
def dist(hist_a, hist_b):
    distance = 0
    for i in range(0, min(len(hist_a), len(hist_b))):
        distance += (hist_a[i] - hist_b[i])**2

    distance = math.sqrt(distance)
    return distance

# Clustering done if iterations >= max_iter or centers have converged
def done(old_cent, new_cent, iterations, max_iterations):
    if(iterations >= max_iterations):
        return True
    for i in range(0, len(old_cent)):
        if(dist(old_cent[i], new_cent[i]) > 0):
            return False
    return True

# Assign labels to each histogram based on distance from centers
def getLabels(F, centers, labels):
    for k in range(0, len(F)):
        #min = dist(F[k], centers[0])
        min = abs(F[k][0] - centers[0][0])
        index = 0
        for i in range(1, len(centers)):
            #d = dist(F[k], centers[i])
            d = abs(F[k][0] - centers[i][0])
            if(d <= min):
                min = d
                index = i
        labels[k] = index

# Get new centers based on the average of a cluster's points
# If a cluster is empty, have to assign a new random center
def getCenters(F, labels, num_clusters):
    c = [[0 for _ in range(len(F[0]))] for _ in range(len(F))]      # List of centers, each is a histogram
    members = len(F) * [0]
    
    # Sum each histogram's strides to get a total histogram for each label i
    for i in range(0, len(F)):
        label = labels[i]
        center = c[label]  # Corresponding center for F[i]
        members[label] += 1      # Increment number of elements in label i
        for j in range(0, len(F[i])):
            center[j] += F[i][j]     # Add j component

    # Get averages
    for i in range(0, len(c)):
        size = members[i]
        # If cluster is empty, make a new "random" center
        if(size == 0):
            r = numpy.random.randint(0, len(F) - 1)     # Get a random histogram, assign as center
            for j in range(0, len(F[r])):
                c[i][j] = F[r][j]
        else:
            for freq in c[i]:                     # Compute averages
                freq /= size
    return c

# K-means cluster the set of histograms F into num_clust clusters with max_iter iterations
def cluster(F, num_clust, max_iter):
    # Generate "random" centers to start
    cent = [[0 for _ in range(len(F[0]))] for _ in range(len(F))]      # List of centers, each is a histogram
    old_cent = [[1 for _ in range(len(F[0]))] for _ in range(len(F))]      # List of centers, each is a histogram

    # Set each interval to be labeled as random phase
    labels = len(F) * [0]
    for i in range(0, len(F)):
        labels[i] = numpy.random.randint(0, num_clust - 1)
        cent[i] = [numpy.random.randint(0, 10*F[i][0]) for j in range(0, len(cent[i]))]

    #print cent
    #print old_cent

    # Iterate through clustering until done
    iter = 0
    while(not done(old_cent, cent, iter, max_iter)):
        # Save state
        old_cent = copy.deepcopy(cent)
        iter += 1

        # Assign phase labels to each histogram based on which center is closest
        getLabels(F, cent, labels)

        # Get new centers
        cent = getCenters(F, labels, num_clust)
        #print cent
        #print labels

    # Return the centers - the averages of each phase type
    return cent

def main():
    # Read in data
    F = []
    with open ('input.txt') as file:
        temp = []
        for line in file:
            temp.append(int(line))
            # Size of histogram
            if(len(temp) >= 6):
                F.append(copy.deepcopy(temp))
                temp = []

    # Phase clustering
    # Group like phases together to get overall behavior
    avgs = cluster(F, 8, 10) # 8 clusters, 10 iterations

    # Define constants
    # size of histrogram x-axis
    k = len(avgs)
    # privacy level
    epsilon = 1
    _lambda = math.sqrt(k) * l2_sensitivity() / epsilon
    
    output = []
    # Iterate through the phase averages
    for F_k in avgs:
        temp = []
        # Calculate LPA(F^k, \lambda)
        for f in F_k:
            # Laplace perturbation
            noise = numpy.random.laplace(0, _lambda)
            temp.append(f + noise)
 
        # Generate probablity model
        length = len(temp)
        for i in range(0, length):
            temp[i] = int(temp[i] * 100 / length)

        output.append(copy.deepcopy(temp))
 
        # TODO: What if probabilities are negative??

    print output

main()
