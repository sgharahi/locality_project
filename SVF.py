#! /usr/bin/python

import math
import numpy

v_sim = [[]]
a_sim = [[]]

def gen_smat_v(int_a, int_b, i, j):
    v_sim[i][j] = DTW(int_a, int_b)

def gen_smat_a(int_a, int_b, i, j):
    a_sim[i][j] = abs(int_a - int_b)

def DTW(int_a, int_b):
    dtw = numpy.zeroes(len(int_a), len(int_b))
    

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

    i = 0
    for v_int in v_trace:
        for j in range(0, len(v_trace)):
        gen_smat(v_int, v_trace[j], i, j, v_sim)
        j = j + 1

    i = 0
    for a_int in a_trace:
        for j in range(0, len(a_trace)):
        gen_smat_a(a_int, a_trace[j], i, j, a_sim)
        j = j + 1

main()
