#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string>
#include <thrust/device_vectorh>
#include <string.h>

using namespace thrust;

cudaSVF(device_vector<device_vector<int>> v_trace, device_vector<int> a_trace);

int main(int argc, char** argv)
{
    device_vector<device_vector<int>> v_sim;
    device_vector<device_vector<int>> a_sim;

    device_vector<vector<int>> v_trace;
    device_vector<int> a_trace;
    
    FILE *v_fp, *a_fp;

    v_fp = fopen("mem_trace.out", "r");
    a_fp = fopen("dcache_trace.out", "r");

    ssize_t read;
    char* line;
    size_t len;

    while((read = getLine(&line, &len, v_fp)) != -1)
    {
        device_vector<int> interval;
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
