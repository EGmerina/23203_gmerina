#include <stdio.h>

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#define MEASUREMENTS_NUMBER 20
#define K 10
#define OFFSET 6 * 1024 * 1024 / sizeof(int)
#define SIZE 6 * 1024 * 1024 / sizeof(int)

typedef union ticks
{
    unsigned long long t64;
    struct s32
    {
        long th, tl;
    } t32;
} ticks;
ticks start, end;

void print_arr(int *arr, size_t N) // печатает обход массива
{
    size_t ind = 0;
    for (size_t i = 0; i < SIZE + 1; ++i)
    {
        printf("%ld ", ind);
        ind = arr[ind];
    }
    printf("\n");
}

void warmup_cache(int *arr)
{
    size_t ind = 0;
    for (size_t i = 0; i < SIZE; ++i)
    {
        ind = arr[ind];
    }
    if (ind == 3)
        printf("Wow! %ld\n", ind);
}

void fix_proc_frequency()
{
    const int N = 100;
    float *A = calloc(N * N, sizeof(float));
    assert(A != NULL);
    float *B = calloc(N * N, sizeof(float));
    assert(B != NULL);
    float *res = calloc(N * N, sizeof(float));
    assert(res != NULL);
    for (size_t i = 0; i < N * N; i++)
    {
        A[i] = i;
        B[i] = i + 1;
    }

    for (size_t i = 0; i < N; i++)
    {
        float *d = res + i * N;
        for (size_t cnt = 0; cnt < N; cnt++)
        {
            const float *b = B + cnt * N;
            float a = A[i * N + cnt];
            for (size_t j = 0; j < N; j++)
            {
                d[j] += a * b[j];
            }
        }
    }

    printf("matrix : %f\n", A[0]);
    free(A);
    free(B);
    free(res);
}
///////////////////////////

void fill_arr(int *arr, size_t N)
{
    for (size_t i = 0; i < N - 1; i++) // заполняем первые N-1 фрагментов
    {
        for (size_t j = 0; j < SIZE / N; j++)
        {
            arr[i * OFFSET + j] = OFFSET + (i * OFFSET + j);
        }
    }
    for (size_t j = 0; j < SIZE / N - 1; j++) // заполняем последний фрагмент
    {
        arr[OFFSET * (N - 1) + j] = j + 1;
    }
    arr[OFFSET * (N - 1) + SIZE / N - 1] = 0; // заполняем последний элемент
}

void traverse(size_t N_min, size_t N_max)
{
    printf("######## traverse #########\n");
    printf("N  tacts\n");
    for (size_t N = N_min; N <= N_max; ++N)
    {
        int *arr = calloc(N * OFFSET, sizeof(int));
        assert(arr != NULL);
        fill_arr(arr, N);
        // print_arr(arr, N);
        warmup_cache(arr);

        unsigned long long result_ticks = __INT64_MAX__;
        for (size_t i = 0; i < MEASUREMENTS_NUMBER; i++)
        {

            asm("rdtsc\n" : "=a"(start.t32.th), "=d"(start.t32.tl));

            size_t ind = 0;
            for (size_t i = 0; i < SIZE * K; ++i)
            {
                ind = arr[ind];
            }

            asm("rdtsc\n" : "=a"(end.t32.th), "=d"(end.t32.tl));

            if (ind == 3)
                printf("Wow! %ld\n", ind);

            long long time_on_elem = (end.t64 - start.t64) / (SIZE * K);
            result_ticks = (result_ticks < time_on_elem) ? result_ticks : time_on_elem;
        }

        printf("%ld %lld\n", N, result_ticks);

        free(arr);
    }
}

int main(void)
{
    const size_t N_min = 1;
    const size_t N_max = 32;

    fix_proc_frequency();

    traverse(N_min, N_max);
}