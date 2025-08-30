#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <float.h>

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
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        printf("parallel dot to dot program\n");
        // printf("amount of processes %d\n", size);
        printf("N : %d\n", N);
        long long s = 0;
        int *a;
        int *b;
        intit_vectors(&a, &b);

        double start, end;
        double total_time = DBL_MAX;

        int size_of_one_part_a = N / size;
        int size_of_last_part = N % size + size_of_one_part_a;

        start = MPI_Wtime();
        for (size_t i = 1; i < size; i++)
        {

            if (i != size - 1)
            {
                int size_a = size_of_one_part_a;
                MPI_Send(&size_a, 1, MPI_INT, i, 11, MPI_COMM_WORLD);
                MPI_Send(a + i * size_of_one_part_a, size_of_one_part_a, MPI_INT, i, 2, MPI_COMM_WORLD);
            }
            else
            {
                int size_a = size_of_last_part;
                MPI_Send(&size_a, 1, MPI_INT, i, 11, MPI_COMM_WORLD);
                MPI_Send(a + i * size_of_one_part_a, size_of_last_part, MPI_INT, i, 2, MPI_COMM_WORLD);
            }
            MPI_Send(b, N, MPI_INT, i, 1, MPI_COMM_WORLD);
        }

        for (size_t i = 0; i < N; i++)
        {
            for (size_t j = 0; j < size_of_one_part_a; j++)
            {
                s += a[j] * b[i];
            }
        }

        long long rec_part_s = 0;
        for (size_t i = 1; i < size; i++)
        {
            MPI_Recv(&rec_part_s, 1, MPI_LONG, i, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            s += rec_part_s;
        }
        end = MPI_Wtime();
        total_time = end - start ;

        printf("Sum : %lld\n", s);
        printf("Time taken: %lf sec.\n", total_time);
        deinit_vectors(&a, &b);
    }
    else
    {

        long long part_s = 0;
        int size_part_a;

        MPI_Recv(&size_part_a, 1, MPI_INT, 0, 11, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int *rec_a = calloc(size_part_a, sizeof(int));
        int *rec_b = calloc(N, sizeof(int));

        MPI_Recv(rec_a, size_part_a, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(rec_b, N, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (size_t i = 0; i < N; i++)
        {
            for (size_t j = 0; j < size_part_a; j++)
            {
                part_s += rec_a[j] * rec_b[i];
            }
        }

        MPI_Send(&part_s, 1, MPI_LONG, 0, 3, MPI_COMM_WORLD);
        free(rec_a);
        free(rec_b);
    }
    MPI_Finalize();
    return 0;
}
