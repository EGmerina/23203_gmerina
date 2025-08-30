#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define SIZE 32768 + 524288 + 4194304 / sizeof(int)
#define OFFSET 16 * 1024 * 1024 // 16 MB

double count(int* arr, size_t size) {
    union ticks {
        unsigned long long t64;
        struct { long th, tl; } t32;
    } start, end;

    double min_time = -1;

    for (size_t l = 0; l < 10; l++) {
        int k = 0;

        // Запуск таймера
        asm("rdtsc" : "=a"(start.t32.th), "=d"(start.t32.tl));

        // Основной цикл
        for (size_t i = 0; i < size; i++) {
            k = arr[k];
        }

        // Остановка таймера
        asm("rdtsc" : "=a"(end.t32.th), "=d"(end.t32.tl));

        // Вычисление времени выполнения
        double elapsed = (double)(end.t64 - start.t64);

        // Обновление минимального времени
        if (min_time == -1 || elapsed < min_time) {
            min_time = elapsed;
        }
    }

    // Возвращение среднего времени на одну итерацию
    return min_time / size;
}


int main() {
    FILE* file = fopen("result.csv", "w"); // Открываем файл для записи
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    fprintf(file, "Fragments,Ticks\n"); // Заголовок CSV

    // Инициализация массива
    for (int i = 1; i <= 32; i++) {
        int* array = (int*)malloc(i * OFFSET * sizeof(int));
        if (!array) {
            perror("Failed to allocate memory");
            fclose(file);
            return 1;
        }
        for (int j = 0; j < i - 1; j++) {
            for (int k = 0; k < SIZE / i; k++) {
                array[(OFFSET / sizeof(int)) * j + k] = (j + 1) * (OFFSET / sizeof(int)) + k;//указываем на следующий индекс 
               
            }
        }
        for (int k = 0; k < SIZE / i; k++) {
            array[(OFFSET / sizeof(int)) * (i - 1) + k] = k + 1; // у последнего фрагмента нет последующего фрагмента, 
          
        }
            

        double result = count(array, SIZE * i);
        fprintf(file, "%d,%f\n", i, result); // Запись результата в файл

        free(array);
    }

    fclose(file); // Закрываем файл
    return 0;
}
