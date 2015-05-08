#! /usr/bin/python

import math
import numpy as np
import sys

# Dynamic Time Warping (distance) of two trace vectors
def DTW(vector_a, vector_b):
    dtw = np.zeros((len(vector_a), len(vector_b)))

    for i in range(0, len(vector_a)):
        dtw[i][0] = sys.maxsize
    for i in range(0, len(vector_b)):
        dtw[0][i] = sys.maxsize

    dtw[0][0] = 0

    for i in range(1, len(vector_a)):
        for j in range(1, len(vector_b)):
            cost = abs(vector_a[i] - vector_b[j])
            dtw[i][j] = cost + min([dtw[i-1][j], dtw[i][j-1], dtw[i-1][j-1]])

    ret = dtw[len(vector_a) - 1][len(vector_b) - 1] / (len(vector_a)*len(vector_b))

    return ret
    

def sensitivity():
    file_vector = []
    for i in range (1, len(sys.argv)):
        file = sys.argv[i]
        with open(file) as f:
            vector = []
            for line in f:
                vector.append(int(line))
            file_vector.append(vector)

    max = 0
    for i in range(0, len(file_vector)):
        for j in range(i + 1, len(file_vector)):
            out = DTW(file_vector[i], file_vector[j])
            if(out > max):
                max = out

    print "Max:", int(max)
