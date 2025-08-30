#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <immintrin.h>

char get_matrix(float *A, FILE *fin, const size_t N)
{
    size_t i = 0;
    while (!feof(fin))
    {
        if (i == N * N)
        {
            return 1;
        }
        fscanf(fin, "%f", A + i);

        ++i;
    }
    if (i < N * N)
    {
        return 1;
    }
    return 0;
}

float *unit_matrix(const size_t N)
{
    float *matr = calloc(N * N, sizeof(float));
    assert(matr != NULL);
    for (size_t i = 0; i < N; ++i)
    {
        matr[i * N + i] = 1;
    }
    return matr;
}

void transpose(float *dest, const float *src, const size_t N)
{
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            dest[i * N + j] = src[j * N + i];
        }
    }
}

void mult(float *A, const float *B, const size_t N)
{
    float *dest_buf = calloc(N * N, sizeof(float));
    assert(dest_buf != NULL);

    for (size_t i = 0; i < N; i++)
    {
        float *d = dest_buf + i * N;
        for (size_t cnt = 0; cnt < N; cnt++)
        {
            const float *b = B + cnt * N;
            float a = A[i * N + cnt];

            size_t j = 0;
            for (; j + 7 < N; j += 8)
            {
                __m256 a_vec = _mm256_set1_ps(a);      // вектор из 8 элементов, всех равных a.
                __m256 b_vec = _mm256_loadu_ps(b + j); // загружает 8 элементов из массива b в вектор.
                __m256 d_vec = _mm256_loadu_ps(d + j);

                __m256 mul_res = _mm256_mul_ps(a_vec, b_vec);
                __m256 add_res = _mm256_add_ps(mul_res, d_vec);

                _mm256_storeu_ps(d + j, add_res);
            }

            // Обработка оставшихся элементов, если N не кратно 8
            for (; j < N; j++)
            {
                d[j] += a * b[j];
            }
        }
    }

    memcpy(A, dest_buf, N * N * sizeof(float)); // результат лежит в A
    free(dest_buf);
}

void add(float *dest, const float *src, const size_t N)
{
    size_t i = 0;
    for (; i + 7 < N * N; i += 8)
    {
        __m256 dest_vec = _mm256_loadu_ps(dest + i);
        __m256 src_vec = _mm256_loadu_ps(src + i);
        __m256 res_vec = _mm256_add_ps(dest_vec, src_vec);
        _mm256_storeu_ps(dest + i, res_vec);
    }

    for (; i < N * N; i++)
    {
        dest[i] = dest[i] + src[i];
    }
}

void sub(float *dest, const float *src, const size_t N)
{
    size_t i = 0;
    for (; i + 7 < N * N; i += 8)
    {
        __m256 dest_vec = _mm256_loadu_ps(dest + i);
        __m256 src_vec = _mm256_loadu_ps(src + i);
        __m256 res_vec = _mm256_sub_ps(dest_vec, src_vec);
        _mm256_storeu_ps(dest + i, res_vec);
    }

    for (; i < N * N; i++)
    {
        dest[i] = dest[i] - src[i];
    }
}

float max_col_sum(const float *A, const size_t N)
{
    float mxsum = 0;
    for (size_t j = 0; j < N; j++)
    {
        float sum = 0;
        for (size_t i = 0; i < N; i++)
        {
            float a = (A[i * N + j] <= 0) ? -A[i * N + j] : A[i * N + j];
            sum += a;
        }
        mxsum = (mxsum < sum) ? sum : mxsum;
    }
    return mxsum;
}

float max_str_sum(const float *A, const size_t N)
{
    float mxsum = 0;
    for (size_t i = 0; i < N; i++)
    {
        float sum = 0;
        for (size_t j = 0; j < N; j++)
        {
            float a = (A[i * N + j] <= 0) ? -A[i * N + j] : A[i * N + j];
            sum += a;
        }
        mxsum = (mxsum < sum) ? sum : mxsum;
    }
    return mxsum;
}

void div_matrix(float *B, const float divider, const size_t N)
{
    __m256 divider_vec = _mm256_set1_ps(divider);

    size_t i = 0;
    for (; i + 7 < N * N; i += 8)
    {
        __m256 B_vec = _mm256_loadu_ps(B + i);
        __m256 res_vec = _mm256_div_ps(B_vec, divider_vec);
        _mm256_storeu_ps(B + i, res_vec);
    }

    for (; i < N * N; i++)
    {
        B[i] = B[i] / divider;
    }
}

float *get_B_matrix(const float *A, const size_t N)
{
    float *B = calloc(N * N, sizeof(float));
    assert(B != NULL);
    transpose(B, A, N);
    float a_1 = max_col_sum(A, N);
    float a_inf = max_str_sum(A, N);
    assert(a_1 != 0 && a_inf != 0);
    div_matrix(B, a_1 * a_inf, N);
    return B;
}

float *get_R_matrix(const float *A, const float *B, const size_t N)
{
    float *R = unit_matrix(N);
    float *B_buf = calloc(N * N, sizeof(float));
    assert(B_buf != NULL);
    memcpy(B_buf, B, N * N * sizeof(float));
    mult(B_buf, A, N);
    sub(R, B_buf, N);
    free(B_buf);
    return R;
}

void invert_matrix(const float *A, float *inv_A, const size_t N, const size_t M)
{
    float *summand = unit_matrix(N);

    float *B = get_B_matrix(A, N);

    float *R = get_R_matrix(A, B, N);

    for (size_t step = 0; step < M; ++step)
    {
        add(inv_A, summand, N);
        mult(summand, R, N);
    }

    mult(inv_A, B, N);

    free(B);
    free(R);
    free(summand);
}

void print_matrixs_beg(float *inv_A, size_t N)
{
    printf("Левый верхний угол матрицы : \n");
    size_t size = (N < 4) ? N : 4;
    for (size_t i = 0; i < size; ++i)
    {
        for (size_t j = 0; j < size; ++j)
        {
            printf("%f ", inv_A[i * N + j]);
        }
        printf("\n");
    }
}

void generate_matrix(const size_t N, const size_t method)
{
    if (method == 0)
    {
        return;
    }

    FILE *fout = fopen("matr.txt", "w");

    switch (method)
    {
    case 1:
        for (size_t i = 0; i < N; i++)
        {
            for (size_t j = 0; j < N; j++)
            {
                if (j < i)
                {
                    fprintf(fout, " %d", 0);
                }
                else
                {
                    fprintf(fout, " %ld", i * N + j + 1);
                }
            }
        }
        break;

    case 2:
        for (size_t i = 0; i < N * N; i++)
        {
            if (i % (N + 1) == 0)
            {
                fprintf(fout, " %f", (i + 1) * 0.01);
                continue;
            }
            fprintf(fout, " %d", 0);
        }
        break;

    default:
        printf("выберете существующий метод\n");
        break;
    }

    fclose(fout);
}

int main(int argc, char *argv[])
{
    size_t N, M, method;
    printf("Введите размер матрицы : ");
    scanf("%lu", &N);
    printf("Введите число членов ряда : ");
    scanf("%lu", &M);

    printf("Введите номер метода генерации матрицы : ");
    scanf("%lu", &method);

    generate_matrix(N, method);

    FILE *fin = fopen("matr.txt", "r");

    float *A = calloc(N * N, sizeof(float));
    if (NULL == A)
    {
        printf("ошибка выделения памяти для A\n");
        fclose(fin);
        return 0;
    }

    float *inv_A = calloc(N * N, sizeof(float));
    if (NULL == inv_A)
    {
        printf("ошибка выделения памяти для inv_A\n");
        free(A);
        fclose(fin);
        return 0;
    }

    if (get_matrix(A, fin, N))
    {
        printf("Размер матрицы не соответствует количеству введенных данных\n");
        free(A);
        free(inv_A);
        fclose(fin);
        return 0;
    }

    printf("Исходная матрица : ");
    print_matrixs_beg(A, N);

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    invert_matrix(A, inv_A, N, M);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    printf("Обратная матрица : ");
    print_matrixs_beg(inv_A, N);

    printf("Затраченное время : %lf sec.\n",
           end.tv_sec - start.tv_sec + 0.000000001 * (end.tv_nsec - start.tv_nsec));

    free(A);
    free(inv_A);
    fclose(fin);
}
