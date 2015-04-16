#! /usr/bin/python

import math
import numpy
import sys

v_sim = [[]]
a_sim = [[]]

def gen_smat_v(int_a, int_b, i, j):
    v_sim[i][j] = DTW(int_a, int_b)

def gen_smat_a(int_a, int_b, i, j):
    a_sim[i][j] = abs(int_a - int_b)

def DTW(int_a, int_b):
    dtw = numpy.zeroes(len(int_a), len(int_b))

    for i in range(0, len(int_a)):
        dtw[i][0] = sys.maxsize
    for i in range(0, len(int_b)):
        dtw[0][i] = sys.maxsize

    dtw[0][0] = 0

    for i in range(1, len(int_a)):
        for j in range(1, len(int_b)):
            cost = abs(int_a[i] - int_b[j])
            dtw[i][j] = cost + min([dtw[i-1][j], dtw[i][j-1], dtw[i-1][j-1]])

    return dtw[len(int_a)][len(int_b)]

def SVF():

def main():
    v_trace = [[]]
    a_trace = []
    
    v_file = "mem_trace.out"
    a_file = "dcache_trace.out"

    with open(v_file) as f:
        for line in f:
            next = line.split(" ")
            v_trace.append(next)
    
    with open(a_file) as f:
        for next in f:
            a_trace.extend(next)

    for i in range(0, len(v_trace)):
        for j in range(0, len(v_trace)):
            gen_smat_v(v_trace[i], v_trace[j], i, j, v_sim)

    for i in range(0, len(a_trace)):
        for j in range(0, len(a_trace)):
            gen_smat_a(a_trace[i], a_trace[j], i, j, a_sim)

main()
