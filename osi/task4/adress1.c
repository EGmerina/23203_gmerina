#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int global_initialized = 10;

int global_uninitialized;

const int global_const = 20;

void print_addresses()
{

    int local_var = 30;

    static int static_var = 40;

    const int local_const = 50;

    printf("Локальная переменная в функции: %p\n", (void *)&local_var);
    printf("Статическая переменная в функции: %p\n", (void *)&static_var);
    printf("Константа в функции: %p\n", (void *)&local_const);
}

int *return_local_address()
{
    int local = 123;
    printf("d. Адрес локальной переменной внутри функции: %p\n", (void *)&local);
    return &local;
}

void heap_operations()
{
    printf("\ne. Работа с кучей:\n");

   
    char *buf1 = (char *)malloc(100);
    strcpy(buf1, "Hello world");
    printf("Содержимое буфера: %s\n", buf1);
    free(buf1);

   
    printf("После освобождения: %s\n", buf1);

    char *buf2 = (char *)malloc(100);
    strcpy(buf2, "Another buffer");
    printf("Новый буфер: %s\n", buf2);

   
    char *mid = buf2 + 50;
   // free(mid);
    free(buf2);
    
    printf("После неправильного освобождения: %s\n", buf2);
    
}


void env_vars() {
    printf("\nh. Работа с переменными окружения:\n");
   
    char* env = getenv("MY_ENV_VAR");
    printf("Исходное значение: %s\n", env);
    
    setenv("MY_ENV_VAR", "NEW_VALUE", 1);
    
    env = getenv("MY_ENV_VAR");
    printf("Новое значение: %s\n", env);
}

int main()
{
    printf("Глобальная инициализированная переменная: %p\n", (void *)&global_initialized);
    printf("Глобальная неинициализированная переменная: %p\n", (void *)&global_uninitialized);
    printf("Глобальная константа: %p\n", (void *)&global_const);
    print_addresses();

    int *local_ptr = return_local_address();
    printf("d. Адрес после возврата: %p\n", (void *)local_ptr);

    heap_operations();

    env_vars();

    printf("pid : %d\n", getpid());

    sleep(30);

    return 0;
}