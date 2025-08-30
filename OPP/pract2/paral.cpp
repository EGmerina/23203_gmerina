#include <iostream>
#include <math.h>
#include <float.h>
#include <mpi.h>

#define N 1000
#define E 0.00001
#define T 0.001
#define CICLES_NUM 3

void print_matrix(double *matrix)
{
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            std ::cout << matrix[i * N + j] << " ";
        }
        std::cout << std::endl;
    }
}

void print_vec(double *vec)
{
    for (size_t i = 0; i < N; i++)
    {
        std ::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}
/// //////////////////////////////////////////

void init_matrix(double *matrix)
{
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            if (i == j)
            {
                matrix[i * N + j] = N;
            }
            else
            {
                matrix[i * N + j] = 1;
            }
        }
    }
}

void init_vector(double *vec)
{
    for (size_t i = 0; i < N; i++)
    {
        vec[i] = 2;
    }
}

void init_x0(double *vec)
{
    for (size_t i = 0; i < N; i++)
    {
        vec[i] = 1;
    }
}

double norm(const double *vec)
{
    double res = 0;
    for (size_t i = 0; i < N; i++)
    {
        res += (vec[i] * vec[i]);
    }
    return res;
}

double *sub(double *vec1, const double *vec2, int size) // результат в vec1
{
    for (size_t i = 0; i < size; i++)
    {
        vec1[i] = vec1[i] - vec2[i];
    }
    return vec1;
}

double *const_mult(double c, double *vec, int size)
{
    for (size_t i = 0; i < size; i++)
    {
        vec[i] = c * vec[i];
    }
    return vec;
}

double *mult(const double *matrix, double *vec, int col_size) // результат в vec
{
    double *res = new double[N];
    std::fill(res, res + N, 0);
    for (size_t i = 0; i < col_size; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            res[i] += matrix[i * N + j] * vec[j];
        }
    }

    std::copy(res, res + N, vec);

    delete[] res;
    return vec;
}

int main(int argc, char *argv[])
{
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int num_lines = (N % size == 0) ? N : size * (N / size + 1); // количество строк кратно size
    double *matrix;
    double *b_vec;
    double E_b_norm_2 = 0;

    if (rank == 0)
    {

        matrix = new double[num_lines * N];
        std::fill(matrix, matrix + N * num_lines, 0);
        init_matrix(matrix);
        b_vec = new double[num_lines];
        std::fill(b_vec, b_vec + num_lines, 0);
        init_vector(b_vec);
        E_b_norm_2 = pow(norm(b_vec) * E, 2);
        // print_matrix(matrix);
        // print_vec(b_vec);
    }

    double *x = new double[num_lines];

    double *buf = new double[num_lines];

    int cnt = 0;

    int part_vec_size = num_lines / size;
    int size_part_matrix = part_vec_size * N;

    double *part_of_matrix = new double[size_part_matrix];
    double *part_b_vec = new double[part_vec_size];
    double *part_x_vec = new double[part_vec_size];

    double start, end;
    double total_time = DBL_MAX;

    for (size_t k = 0; k < CICLES_NUM; k++)
    {
        std::fill(x, x + num_lines, 0);
        init_x0(x);
        std::copy(x, x + N, buf);
        cnt = 0;

        double norm_Ax_b = norm(x);

        start = MPI_Wtime();

        MPI_Bcast(&E_b_norm_2, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatter(b_vec, part_vec_size, MPI_DOUBLE, part_b_vec, part_vec_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatter(matrix, size_part_matrix, MPI_DOUBLE, part_of_matrix, size_part_matrix, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        while (norm_Ax_b >= E_b_norm_2)
        {
            cnt++;
            if (cnt > 50000)
            {
                std::cout << "последовательность расходится" << std::endl;
                break;
            }

            std::copy(x + part_vec_size * rank, x + part_vec_size * (rank + 1), part_x_vec);

            sub(mult(part_of_matrix, buf, part_vec_size), part_b_vec, part_vec_size);


            MPI_Allgather(buf, part_vec_size, MPI_DOUBLE, x, part_vec_size, MPI_DOUBLE, MPI_COMM_WORLD); //тут лучше бы использовать Reduce (каждый вектор считает сумму квадратов своей части)
            norm_Ax_b = norm(x);

            sub(part_x_vec, const_mult(T, buf, part_vec_size), part_vec_size);

            MPI_Allgather(part_x_vec, part_vec_size, MPI_DOUBLE, x, part_vec_size, MPI_DOUBLE, MPI_COMM_WORLD);

            std::copy(x, x + N, buf);
        }
        end = MPI_Wtime();
        total_time = (end - start < total_time) ? end - start : total_time;
    }
    if (cnt < 3)
    {
        std::cout << "последовательность сходится слишком быстро" << std::endl;
    }
    if (rank == 0)
    {
        //print_vec(x);

        std::cout << "time : " << total_time << " proc : " << size << " counter : " << cnt << std::endl;
        delete[] matrix;
        delete[] b_vec;
    }

    delete[] part_x_vec;
    delete[] part_b_vec;
    delete[] buf;
    delete[] x;
    delete[] part_of_matrix;

    MPI_Finalize();
}