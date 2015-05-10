#! /usr/bin/python

import math
import numpy as np
import sys

def sensitivity(files):
    file_vector = []
    for file in files:
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
    return max
