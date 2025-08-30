#include <iostream>
#include <math.h>
#include <float.h>
#include <omp.h>

#define N 1000
#define E 0.00001
#define T 0.001
#define CICLES_NUM 1

void print_matrix(double *matrix) {
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            std::cout << matrix[i * N + j] << " ";
        }
        std::cout << std::endl;
    }
}

void print_vec(double *vec) {
    for (size_t i = 0; i < N; i++) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}

void init_matrix(double *matrix) {
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            if (i == j) {
                matrix[i * N + j] = N;
            } else {
                matrix[i * N + j] = 1;
            }
        }
    }
}

void init_vector(double *vec) {
    for (size_t i = 0; i < N; i++) {
        vec[i] = 2;
    }
}

void init_x0(double *vec) {
    for (size_t i = 0; i < N; i++) {
        vec[i] = 1;
    }
}

double norm(const double *vec) {
    double res = 0;
    #pragma omp parallel for reduction(+:res)
    for (size_t i = 0; i < N; i++) {
        res += (vec[i] * vec[i]);
    }
    return res;
}

double *sub(double *vec1, const double *vec2) {
    #pragma omp parallel for
    for (size_t i = 0; i < N; i++) {
        vec1[i] = vec1[i] - vec2[i];
    }
    return vec1;
}

double *const_mult(double c, double *vec) {
    #pragma omp parallel for
    for (size_t i = 0; i < N; i++) {
        vec[i] = c * vec[i];
    }
    return vec;
}

double *mult(const double *matrix, double *vec) {
    double *res = new double[N];
    std::fill(res, res + N, 0);
    #pragma omp parallel for
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            res[i] += matrix[i * N + j] * vec[j];
        }
    }
    std::copy(res, res + N, vec);
    delete[] res;
    return vec;
}

int main(int argc, char *argv[]) {
    double *matrix = new double[N * N];
    double *b_vec = new double[N];
    init_matrix(matrix);
    init_vector(b_vec);

    double *x = new double[N];
    double *buf = new double[N];

    int cnt = 0;
    double E_b_norm_2 = pow(norm(b_vec) * E, 2);

    double start, end;
    double total_time = DBL_MAX;

    for (size_t k = 0; k < CICLES_NUM; k++) {
        init_x0(x);
        std::copy(x, x + N, buf);
        cnt = 0;
        double norm_Ax_b = norm(x);

        start = omp_get_wtime();

        #pragma omp parallel
        {
            while (true) {
                #pragma omp single
                {
                    norm_Ax_b = norm(sub(mult(matrix, buf), b_vec));
                }

                if (norm_Ax_b < E_b_norm_2) {
                    break;
                }

                #pragma omp single
                {
                    sub(x, const_mult(T, buf));
                    std::copy(x, x + N, buf);
                    cnt++;
                }

                if (cnt > 50000) {
                    #pragma omp single
                    {
                        std::cout << "последовательность расходится" << std::endl;
                    }
                    break;
                }
            }
        }

        end = omp_get_wtime();
        total_time = (end - start < total_time) ? end - start : total_time;
    }

    if (cnt < 3) {
        std::cout << "последовательность сходится слишком быстро" << std::endl;
    }

    print_vec(x);

    std::cout << "time : " << total_time << " counter : " << cnt << std::endl;

    delete[] buf;
    delete[] matrix;
    delete[] b_vec;
    delete[] x;

    return 0;
}