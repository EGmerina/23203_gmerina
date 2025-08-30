#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <mpi.h>

#define CICLES_NUM 3
#define N 100000

void intit_vectors(int **a, int **b)
{
    *a = calloc(N, sizeof(int));
    *b = calloc(N, sizeof(int));
    for (size_t i = 0; i < N; i++)
    {

        (*a)[i] = i;
        (*b)[i] = i;
    }
}

void deinit_vectors(int **a, int **b)
{
    free(*a);
    free(*b);
}

int main(int argc, char *argv[])
{
    printf("sequential program\n");
    printf("N : %d\n", N);

    long long s = 0;
    int *a;
    int *b;
    intit_vectors(&a, &b);

    double start, end;
    double total_time = DBL_MAX;

    MPI_Init(&argc, &argv);
    for (size_t k = 0; k < CICLES_NUM; k++)
    {
        start = MPI_Wtime();
        for (size_t i = 0; i < N; i++)
        {
            for (size_t j = 0; j < N; j++)
            {
                s += a[i] * b[j];
            }
        }
        end = MPI_Wtime();
        total_time = (end - start < total_time) ? end - start : total_time;
    }
    MPI_Finalize();
    printf("Sum : %lld\n", s / CICLES_NUM);
    printf("Time taken: %lf sec.\n", total_time);
    deinit_vectors(&a, &b);
    return 0;
}
