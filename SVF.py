#! /usr/bin/python

import math
import numpy as np
import sys

def victim_sim_mat(interval_a, interval_b, i, j, v_sim):
    if(i > j):
        v_sim[i][j] = DTW(interval_a, interval_b)
    else:
        v_sim[i][j] = 0

def attacker_sim_mat(interval_a, interval_b, i, j, a_sim):
    if(i > j):
        a_sim[i][j] = abs(interval_a - interval_b)
    else:
        a_sim[i][j] = 0

def DTW(interval_a, interval_b):
    dtw = np.zeros((len(interval_a), len(interval_b)))
    print interval_a
    print interval_b

    for i in range(0, len(interval_a)):
        dtw[i][0] = sys.maxsize
    for i in range(0, len(interval_b)):
        dtw[0][i] = sys.maxsize

    dtw[0][0] = 0

    for i in range(1, len(interval_a)):
        for j in range(1, len(interval_b)):
            cost = abs(interval_a[i] - interval_b[j])
            dtw[i][j] = cost + min([dtw[i-1][j], dtw[i][j-1], dtw[i-1][j-1]])

    return dtw[len(interval_a)][len(interval_b)]

def SVF(v_sim, a_sim):
    v_mean = np.mean(v_sim)
    a_mean = np.mean(a_sim)
    v_std = np.std(v_sim)
    a_std = np.std(a_sim)

    sum = 0
    for i in range(0, min(len(v_sim), len(a_sim))):
        for j in range(0, min(len(v_sim[0]), len(a_sim[0]))):
            sum += (v_sim[i][j] - v_mean)*(a_sim[i][j] - a_mean)

    sum /= (v_std * a_std)
    return sum

def main():
    v_trace = [[]]
    a_trace = []
    
    v_file = "mem_trace.out"
    a_file = "dcache_trace.out"

    with open(v_file) as f:
        for line in f:
            next = line.strip().split(" ")
            temp = []
            for item in next:
                temp.append(int(item))
            temp = filter(None, temp)
            v_trace.append(temp)
    
    with open(a_file) as f:
        for next in f:
            a_trace.append(int(next))

    print len(v_trace)
    print len(a_trace)

    v_sim = np.zeros((len(v_trace), len(v_trace)))
    a_sim = np.zeros((len(a_trace), len(a_trace)))

    for i in range(0, len(v_trace)):
        for j in range(0, len(v_trace)):
            victim_sim_mat(v_trace[i], v_trace[j], i, j, v_sim)

    for i in range(0, len(a_trace)):
        for j in range(0, len(a_trace)):
            attacker_sim_mat(a_trace[i], a_trace[j], i, j, a_sim)

    print SVF(v_sim, a_sim)

main()
