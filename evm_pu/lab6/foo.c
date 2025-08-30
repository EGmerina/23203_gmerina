#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define N_min 256  // 1 Кб
#define N_max 6291456 // 24  //8388608   // 32 Мб
#define NUM_ITER 10
#define EXPAND_FACT 1.2

union ticks{
    unsigned long long t64;
    struct s32 {
        long th;
        long tl;
    } t32;
} start, end;

void m_mult();
void direct_arr();
void reverse_arr();
void rand_arr();
int bypass(int *arr, const size_t N);

int main(void){
    m_mult();

    direct_arr();
    reverse_arr();
    rand_arr();
    printf("YEEEEEEEEEEP!\n");
    return 0;
}

void m_mult(){
    float* A = calloc(sizeof(float), N_min);
    float* B = calloc(sizeof(float), N_min);
    float* res = calloc (sizeof(float), N_min);

    for (int k = 0; k < N_min/2; ++k){
      for (int i = 0; i < N_min/2; ++i){
        for (int j = 0; j < N_min/2; ++j){               
          res[i * N_min/2 + j] += A[i * N_min/2 + k] * B[k * N_min/2 + j];
        }
      }
    }
    printf("Element of the matrix: %f\n", res[0]);
    free(A);
    free(B);
    free(res);
}

int bypass(int* arr, const size_t N){
    int k = 0;
    for (size_t i = 0; i < N * NUM_ITER; ++i){
        k = arr[k];
    }
    return k;
}

void direct_arr(){
    int tmp = 0;

    printf("\n ================================================== \n");
    printf("DIRECT BYPASS\n");
    for (size_t N = N_min; N <= N_max; N *= EXPAND_FACT){
        int *array = malloc(sizeof(int) * N);
        if (!array)
            return;

        for (int i = 0; i < N - 1; ++i){
            array[i] = i + 1;
        }
        array[N - 1] = 0;

        tmp = bypass(array, N);

        long long min_aver_time = INT_MAX;
        for (int i = 0; i < 5; ++i){
            asm("rdtsc\n" : "=a"(start.t32.th), "=d"(start.t32.tl));
            tmp = bypass(array, N);
            asm("rdtsc\n":"=a"(end.t32.th),"=d"(end.t32.tl));

            long long curr_aver_time = (end.t64 - start.t64) / N / NUM_ITER;
            min_aver_time < curr_aver_time ? min_aver_time : (min_aver_time = curr_aver_time);
        }

        printf("Size of array, time: %lu, %lld\n", N, min_aver_time);
        free(array);
    }
    printf("%d\n", tmp);
}

void reverse_arr(){
    int tmp = 0;

    printf("\n ================================================== \n");
    printf("REVERSE BYPASS\n");
    for (size_t N = N_min; N <= N_max; N *= EXPAND_FACT){
        int *array = malloc(sizeof(int) * N);
        if (!array)
            return;

        for (int i = N - 1; i > 0; --i){
            array[i] = i - 1;
        }
        array[0] = N - 1;

        tmp = bypass(array, N);

        long long min_aver_time = INT_MAX;
        for (int i = 0; i < 5; ++i){
            asm("rdtsc\n" : "=a"(start.t32.th), "=d"(start.t32.tl));
            tmp = bypass(array, N);
            asm("rdtsc\n":"=a"(end.t32.th),"=d"(end.t32.tl));

            long long curr_aver_time = (end.t64 - start.t64) / N / NUM_ITER;
            min_aver_time < curr_aver_time ? min_aver_time : (min_aver_time = curr_aver_time);
        }

        printf("Size of array, time: %lu, %lld\n", N, min_aver_time);
        free(array);
    }
    printf("%d\n", tmp);
}

void rand_arr(){
    int tmp = 0;

    printf("\n ================================================== \n");
    printf("RANDOM BYPASS\n");
    for (size_t N = N_min; N <= N_max; N *= EXPAND_FACT){
        int *array = calloc(sizeof(int), N);
        if (!array)
            return;

        size_t counter = N;
        int iter = 0;
        int elem = 0;
        while (counter){
          elem = rand() % N;
          if (!array[elem]){
            array[iter] = elem;
            iter = elem;
            counter--;
          }
        }

        tmp = bypass(array, N);

        long long min_aver_time = 5000000000000;
        for (int i = 0; i < 20; ++i){
            asm("rdtsc\n" : "=a"(start.t32.th), "=d"(start.t32.tl));
            tmp = bypass(array, N);
            asm("rdtsc\n":"=a"(end.t32.th),"=d"(end.t32.tl));


            long long curr_aver_time = (end.t64 - start.t64) / N / NUM_ITER;
            min_aver_time < curr_aver_time ? min_aver_time : (min_aver_time = curr_aver_time);
        }

        printf("Size of array, time: %lu, %lld\n", N, min_aver_time);
        free(array);
    }
    printf("%d\n", tmp);
}
