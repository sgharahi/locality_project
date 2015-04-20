#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string>
#include <vector>
#include <string.h>

using namespace std;

//void SVF(int** v_simMat, int** a_simMat);
//void genSimilarityMatrix(int* interval_a, int* interval_b, int* p_simMat);

int main(int argc, char** argv)
{
    vector<vector<int>> v_sim;
    vector<vector<int>> a_sim;

    vector<vector<int>> v_trace;
    vector<int> a_trace;
    
    FILE *v_fp, *a_fp;

    v_fp = fopen("mem_trace.out", "r");
    a_fp = fopen("dcache_trace.out", "r");

    ssize_t read;
    char* line;
    size_t len;

    while((read = getLine(&line, &len, v_fp)) != -1)
    {
        vector<int> interval;
        char* token = strtok(line, " ");
        while(token)
        {
            interval.push_back(atoi(token));
            token = strtok(NULL, " ");
        }
        v_trace.push_back(interval);
    }

    while((read = getLine(&line, &len, a_fp)) != -1)
    {
        a_trace.push_back(atoi(line));
    }

    free(line);

    return 0;
}
