#include <iostream>
#include <math.h>
#include <float.h>
#include <omp.h>

#define N 1000
#define E 0.00001
#define T 0.001
#define CICLES_NUM 1

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

int main(int argc, char *argv[])
{

    double *matrix = new double[N * N];
    double *b_vec = new double[N];
    init_matrix(matrix);
    init_vector(b_vec);

    // print_matrix(matrix);
    // print_vec(b_vec);

    double *x = new double[N];

    double *buf = new double[N];

    double *res = new double[N];

    int cnt = 0;

    double E_b_norm_2 = pow(norm(b_vec) * E, 2);

    double start, end;
    double total_time = DBL_MAX;

    for (size_t k = 0; k < CICLES_NUM; k++)
    {
        init_x0(x);
        std::copy(x, x + N, buf);
        cnt = 0;
        double norm_Ax_b = norm(x);

        bool diverged = false;

        start = omp_get_wtime();
#pragma omp parallel
        {
            while (norm_Ax_b >= E_b_norm_2 && !diverged)
            {
                #pragma omp single
                {
                    std::fill(res, res + N, 0);
                }
                #pragma omp for schedule(static)
                for (size_t i = 0; i < N; i++)
                {
                    for (size_t j = 0; j < N; j++)
                    {
                        res[i] += matrix[i * N + j] * buf[j];
                    }
                }
#pragma omp single
                {
                    std::copy(res, res + N, buf);
                }

#pragma omp for schedule(static)
                for (size_t i = 0; i < N; i++)
                {
                    buf[i] = buf[i] - b_vec[i];
                }

                double new_norm = 0;

#pragma omp reduction(+ : new_norm) for schedule(static)
                for (size_t i = 0; i < N; i++)
                {
                    new_norm += (buf[i] * buf[i]);
                }

#pragma omp single
                {
                    norm_Ax_b = new_norm;
                }

#pragma omp for schedule(static)
                for (size_t i = 0; i < N; i++)
                {
                    buf[i] = T * buf[i];
                }

#pragma omp for schedule(static)
                for (size_t i = 0; i < N; i++)
                {
                    x[i] = x[i] - buf[i];
                }
#pragma omp single
                {
                    std::copy(x, x + N, buf);
                    cnt++;
                    if (cnt > 50000)
                    {
                        std::cout << "последовательность расходится" << std::endl;
                        diverged = true;
                    }
                }
            }
        }
        end = omp_get_wtime();
        total_time = (end - start < total_time) ? end - start : total_time;
    }
    if (cnt < 3)
    {
        std::cout << "последовательность сходится слишком быстро" << std::endl;
    }

    print_vec(x);

    std::cout << "time : " << total_time << " counter : " << cnt << std::endl;

    delete[] buf;
    delete[] res;
    delete[] matrix;
    delete[] b_vec;
    delete[] x;
}