#! /usr/bin/python

import math
import numpy as np

def main():
	transition_vector = np.zeros((8,8))
	start_phases = np.zeros(8)	

	with open('input2.txt') as f:
		for line in f:
			next = line.strip().split(" ")
			temp = []
			start = 0
			for item in next:
				temp.append(int(item))
				
			for item in temp:
				if(start == 1):
					prev = current
					current = item
	
					transition_vector[prev][current]+=1
				else:
					start_phases[item]+=1

				current = item
				start = 1	
	

	print transition_vector
	print start_phases

#	input_vector.append(temp)
		 

main()
