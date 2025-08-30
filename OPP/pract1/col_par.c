#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <float.h>

#define N 100000

void intit_vectors(int **a, int **b)
{
    for (size_t i = 0; i < N; i++)
    {

        (*a)[i] = i;
        (*b)[i] = i;
    }
}

void print(int *a, int val)
{
    for (size_t i = 0; i < val; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void deinit_vectors(int **a, int **b)
{
    free(*a);
    free(*b);
}

int main(int argc, char *argv[])
{
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    long long s = 0;
    int *a;
    int *b;
    b = calloc(N, sizeof(int));
    a = calloc(2 * N - N % size, sizeof(int));

    if (rank == 0)
    {
        printf("parallel collective dot program\n");
        // printf("amount of processes %d\n", size);
        printf("N : %d\n", N);
        intit_vectors(&a, &b);
    }

    double start, end;
    double total_time = DBL_MAX;

    int size_of_one_part_a = N / size + (N % size != 0);
    int *rec_a = calloc(size_of_one_part_a, sizeof(int));

    start = MPI_Wtime();

    MPI_Scatter(a, size_of_one_part_a, MPI_INT, rec_a, size_of_one_part_a, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(b, N, MPI_INT, 0, MPI_COMM_WORLD);

    long long local_part_s = 0;
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < size_of_one_part_a; j++)
        {
            local_part_s += rec_a[j] * b[i];
        }
    }

    MPI_Reduce(&local_part_s, &s, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    end = MPI_Wtime();
    total_time = end - start ;

    free(rec_a);

    if (rank == 0)
    {
        printf("Sum : %lld\n", s);
        printf("Time taken: %lf sec.\n", total_time);
        deinit_vectors(&a, &b);
    }

    MPI_Finalize();

    return 0;
}
