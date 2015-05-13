#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include "DTW.h"
#include "mat.h"

using namespace std;

/* Constants and Macros */
#define MIN(x,y) (x < y ? x : y)

/* Check arguments to the program
 * @input:  argc, the number of command line arguments to the program
 */
void check_usage(int argc, char* p)
{
    if(argc < 3)
    {
        fprintf(stderr, "Usage: %s <victim file> <attacker file>\n", p);
        exit(1);
    }
}

void init_sim(int64_t*** v, int64_t*** a, int len)
{
    *a = (int64_t**) malloc(sizeof(int64_t*) * len);
    *v = (int64_t**) malloc(sizeof(int64_t*) * len);

    for(int i = 0; i < len; i++)
    {
        (*v)[i] = (int64_t*) malloc(sizeof(int64_t) * len);
        (*a)[i] = (int64_t*) malloc(sizeof(int64_t) * len);
    }
}

// Create the victim and attacker trace vector and fill with
// values read from file
void fill_vectors(vector<vector<int64_t> > &v_trace, vector<vector<int64_t> > &a_trace, char* v_file, char* a_file)
{
    FILE* vfp = fopen(v_file, "r");
    FILE* afp = fopen(a_file, "r");
    int sentinel = 1;
    char* buff = (char*) malloc(sizeof(char) * 100000);

    // Loop through file to create trace vectors
    while(sentinel)
    {
        // Line buffer
        char* end;

        // Tokenize each line into an array to be added to v_trace
        sentinel = (fgets(buff, 100000, vfp) != NULL);
        char* next_int = strtok(buff, " ");
        vector<int64_t> v;
        while(next_int)
        {
            long l = strtoull(next_int, &end, 10);
            if(l != 0)
                v.push_back(l);
            next_int = strtok(NULL, " ");
        }
        v_trace.push_back(v);

        // Tokenize each line into an array to be added to v_trace
        sentinel &= (fgets(buff, 100000, afp) != NULL);
        next_int = strtok(buff, " ");
        vector<int64_t> a;
        while(next_int)
        {
            long l = strtoull(next_int, &end, 10);
            if(l != 0)
                a.push_back(l);
            next_int = strtok(NULL, " ");
        }
        a_trace.push_back(a);
    }
    fclose(vfp);
    fclose(afp);
    free(buff);
}

// Generate similarity matrix from intervals
void gen_sim(int64_t** v_sim, int64_t** a_sim, vector<vector<int64_t> > &v, vector<vector<int64_t> > &a)
{
    // Calculate the victim similarity based on DTW algorithm
    for(int i = 0; i < v.size(); i++)
        for(int j = 0; j < v.size(); j++)
            if(i < j)
                v_sim[i][j] = (int64_t)DTW(v[i], v[j]);
            else
                v_sim[i][j] = 0;

    // Calculate the attacker similarity based on DTW algorithm
    for(int i = 0; i < a.size(); i++)
        for(int j = 0; j < a.size(); j++)
            if(i < j)
                a_sim[i][j] = (int64_t)DTW(a[i], a[j]);
            else
                a_sim[i][j] = 0;
}

/* Calculate SVF using Pearson correlation between similarity matrices
 * @input:  v, victim similarity matrix
 *          a, attacker similarity matrix
 * @return: SVF score, in range: [-1, 1]
 */
double SVF(int64_t** &v, int v_len, int64_t** &a, int a_len)
{
    double  v_mean, a_mean,
            v_std, a_std;

    v_mean = _mean(v, v_len, v_len);
    a_mean = _mean(a, a_len, a_len);

    v_std = _std(v, v_len, v_len, v_mean);
    a_std = _std(a, a_len, a_len, a_mean);

    double sum = 0;
    int bound = MIN(v_len, a_len);

    for(int i = 0; i < bound; i++)
        for(int j = 0; j < bound; j++)
        {
            sum += (v[i][j] * a[i][j]);
            printf("v[%d][%d]: %lld\t a[%d][%d]: %lld\n", i, j, v[i][j], i, j, a[i][j]);
        }
    printf("Sum: %f\n", sum);
    printf("v_mean: %f\n", v_mean);
    printf("v_std: %f\n", v_std);
    printf("a_mean: %f\n", a_mean);
    printf("a_std: %f\n", a_std);

    sum -= (v_len * a_len * v_mean * a_mean);
    double svf = sum / (v_std * a_std);
    return svf;
}

int main(int argc, char* argv[])
{
    vector< vector<int64_t> > v_trace;
    vector< vector<int64_t> > a_trace;
    int64_t **v_sim, **a_sim;

    // Check command line arguments
    check_usage(argc, argv[0]);

    // Read from victim and attacker files
    printf("Reading input files.\n");
    fill_vectors(v_trace, a_trace, argv[1], argv[2]);

    // Initialize Similarity matrices
    init_sim(&v_sim, &a_sim, v_trace.size());

    // Generate similarity matrices by comparing interval distances
    printf("Generating similarity matrices.\n");
    gen_sim(v_sim, a_sim, v_trace, a_trace);

    // Calculate SVF from similarity matrices
    printf("Computing correlation.\n");
    double result = SVF(v_sim, v_trace.size(), a_sim, a_trace.size());

    printf("Computed SVF: %f\n", ABS(result));
}

