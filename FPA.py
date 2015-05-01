#! /bin/python

# TODO - Find out \Delta_2(Q) (L2 sensitivity of Q)
def l2_sensitivity(Q):
    
    
def main():
    #Read in data
    F_k = []
    with open ("input.txt") as file:
        line = f.read()
        F_k.append(int(line))

    #Define constants
    k = len(F_k)# size of histrogram x-axis
    epsilon = 0.1 #privacy level
    _lambda = Math.sqrt(k) * l2_sensitivity(Q) / epsilon
    
    # Calculate LPA(F^k, \lambda)
    output = []
    for f in F_k:
        # Laplace perturbation
        noise = numpy.random.laplace(0, _lambda)
        output.append(f + noise)

    # Generate probablity model
    len = len(output)
    for i in range(0, len):
        output[len] = int(output[len] * 100 / len)

    # Simulation?
    start = trace.pop(0)
    f_addr = start
    foo(start)
    for r_addr in trace:
        while(true):
            rand = int(Math.rand()*len)
            stride = 0
            while(rand > output[stride] && stride < len - 1):
                rand -= output[stride]
                stride += 1
            f_addr += stride
    
            # TODO - adjust probabilities

            if(f_addr == r_addr):
                foo(r_addr)
                break
            else:
                foo(f_addr)
