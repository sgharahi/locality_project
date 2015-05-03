#! /usr/bin/python

import math
import numpy as np
import sys

def DTW(interval_a, interval_b):
    dtw = np.zeros((len(interval_a), len(interval_b)))

    for i in range(0, len(interval_a)):
        dtw[i][0] = sys.maxsize
    for i in range(0, len(interval_b)):
        dtw[0][i] = sys.maxsize

    dtw[0][0] = 0

    for i in range(1, len(interval_a)):
        for j in range(1, len(interval_b)):
            cost = abs(interval_a[i] - interval_b[j])
            dtw[i][j] = cost + min([dtw[i-1][j], dtw[i][j-1], dtw[i-1][j-1]])

    return dtw[len(interval_a) - 1][len(interval_b) - 1]

def main():

    file_vector = []
    for file in sys.argv:
        with open(file) as f:
            vector = []
            for line in f:
                vector.append(line)
            file_vector.append(vector)
            

    max = 0
    for i in range(0, len(file_vector)):
        for(j in range(i, len(file_fector)):
            out = DTW(file_vector[i], file_vector[j])
            if(out > max)
                max = out

    print max

main()
