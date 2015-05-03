#! /bin/python

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

    return dtw[len(hist_a) - 1][len(hist_b) - 1]


# TODO - Find out \Delta_2(Q) (L2 sensitivity of Q)
# by running DTW on set of input histograms
def l2_sensitivity(Q):
    

def main():
    # Read in data
    F_k = []
    with open ("input.txt") as file:
        line = f.read()
        F_k.append(int(line))

    # Define constants
    k = len(F_k) # size of histrogram x-axis
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
    mem_x(start)
    for r_addr in trace:
        while(true):
            rand = int(Math.rand()*len)
            stride = 0
            while(rand > output[stride] && stride < len - 1):
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
