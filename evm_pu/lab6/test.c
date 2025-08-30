#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#define MEASUREMENTS_NUMBER 10
#define K 10
#define STEP 1.1



typedef union ticks
{
    unsigned long long t64;
    struct s32
    {
        long th, tl;
    } t32;
} ticks;
ticks start, end;

int traverse(int *arr, const size_t N)
{
    int ind = 0;
    for (size_t i = 0; i < N * K; ++i)
    {
        ind = arr[ind];
    }
    return ind;
}

void use_cycle(size_t ind)
{
    if (ind == 3)
        printf("Wow! %ld\n", ind);
}

void warmup_cache(int *arr, size_t N)
{
    size_t ind = 0;
    for (size_t i = 0; i < N; ++i)
    {
        ind = arr[ind];
    }
    use_cycle(ind);
}

void traverse_arr(int *arr, size_t N)
{
    warmup_cache(arr, N);

    unsigned long long result_ticks = __INT64_MAX__;
    for (size_t i = 0; i < MEASUREMENTS_NUMBER; i++)
    {

        asm("rdtsc\n" : "=a"(start.t32.th), "=d"(start.t32.tl));
        size_t last_ind = traverse(arr, N);
        asm("rdtsc\n" : "=a"(end.t32.th), "=d"(end.t32.tl));
        use_cycle(last_ind);
        long long time_on_elem = (end.t64 - start.t64) / N / K;
        result_ticks = (result_ticks < time_on_elem) ? result_ticks : time_on_elem;
    }

    printf("%ld %lld\n", N * sizeof(int) / 1024, result_ticks);
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

void fill_direct(int *arr, size_t N)
{
    for (size_t i = 0; i < N - 1; i++)
    {
        arr[i] = i + 1;
    }
    arr[N - 1] = 0;
}

void traverse_direct(size_t N_min, size_t N_max)
{
    printf("######## traverse direct #########\n");
    printf("size(kb)  tacts\n");
    for (size_t N = N_min; N < N_max; (size_t)(N *= STEP))
    {
        int *arr = calloc(N, sizeof(int));
        assert(arr != NULL);
        fill_direct(arr, N);

        traverse_arr(arr, N);

        free(arr);
    }
}

void fill_reverse(int *arr, size_t N)
{
    for (size_t i = N - 1; i > 0; --i)
    {
        arr[i] = i - 1;
    }
    arr[0] = N - 1;
}

void traverse_reverse(size_t N_min, size_t N_max)
{
    printf("######## traverse reverse #########\n");
    printf("size(kb) tacts\n");
    for (size_t N = N_min; N < N_max; (size_t)(N *= STEP))
    {
        int *arr = calloc(N, sizeof(int));
        assert(arr != NULL);
        fill_reverse(arr, N);

        traverse_arr(arr, N);

        free(arr);
    }
}

int get_random_in_range(unsigned int min, unsigned int max)
{
    return rand() % (max - min + 1) + min;
}

int get_random_num(int *arr, size_t N)
{
    int rd_num = get_random_in_range(0, N - 1);
    int cnt = 0;
    while (arr[rd_num] && cnt < N) // пробуем взять случайное число
    {
        rd_num = get_random_in_range(0, N - 1);
        ++cnt;
    }
    char flag = 0;
    if (arr[rd_num])
    {
        flag = 1;
        for (size_t i = 1; i < N; i++)
        {
            if (arr[i] == 0)
            {
                rd_num = i;
                flag = 0;
                break;
            }
        }
    }
    if (flag)
    {
        return 0;
    }
    return rd_num;
}

void fill_random(int *arr, size_t N)
{
    int ind = 0;
    for (size_t i = 0; i < N - 1; i++)
    {
        int rd_num = get_random_num(arr, N);
        arr[ind] = rd_num;
        ind = rd_num;
    }
}

void traverse_random(size_t N_min, size_t N_max)
{
    printf("######## traverse random #########\n");
    printf("size(kb) tacts\n");
    for (size_t N = N_min; N < N_max; (size_t)(N *= STEP))
    {
        int *arr = calloc(N, sizeof(int));
        assert(arr != NULL);
        fill_random(arr, N);

        traverse_arr(arr, N);

        free(arr);
    }
}

void print_arr(int *arr, size_t N)
{
    size_t ind = 0;
    for (size_t i = 0; i < N; i++)
    {
        ind = arr[ind];
        printf("%ld ", ind);
    }
    printf("\n");
}

int main(void)
{
    const size_t N_min = 1024 * 8 / sizeof(int);
    const size_t N_max = 1024 * 1024 * 18 / sizeof(int);

    fix_proc_frequency();

    traverse_direct(N_min, N_max);
    traverse_reverse(N_min, N_max);
    traverse_random(N_min, N_max);
}