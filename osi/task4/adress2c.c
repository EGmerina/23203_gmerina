#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <signal.h>
#include <string.h>

void segfault_handler(int signum)
{
    printf("Caught SIGSEGV (Segmentation Fault)!\n");
    exit(1);
}

void recursive_stack(int depth)
{
    sleep(3);
    char stack_array[1024 * 1024];
    printf("Stack depth: %d, array at: %p\n", depth, stack_array);
    if (depth < 5)
    {
        recursive_stack(depth + 1);
    }
}

int main()
{
    printf("PID: %d\n", getpid());
    sleep(10);

    recursive_stack(0);

    sleep(10);

    char **pointers_array[5];

    for (int i = 0; i < 5; i++)
    {
        char *heap_buf = malloc(1024 * 1024); // 1 МБ
        pointers_array[i] = heap_buf;
        printf("Heap alloc: %p\n", heap_buf);
        sleep(3);
    }

    printf("Delete memory ......\n");
    sleep(1);
    for (size_t i = 0; i < 5; i++)
    {
        free(pointers_array[i]);
    }

    sleep(5);
    printf("Mapping memory ......\n");
    sleep(5);
    void *mapped = mmap(
        NULL,                        // Адрес (пусть ядро само выберет)
        4096 * 10,                   // Размер (10 страниц)
        PROT_READ | PROT_WRITE,      // Права: чтение + запись
        MAP_ANONYMOUS | MAP_PRIVATE, // Анонимная память (не файл)
        -1,                          // Файловый дескриптор (не нужен)
        0                            // Смещение (не нужно)
    );
    printf("Mapped memory: %p\n", mapped);

    sleep(5);

    signal(SIGSEGV, segfault_handler);

    mprotect(mapped, 4096 * 10, PROT_NONE);

     *(int *)mapped = 123; // SIGSEGV (нарушение прав доступа)
    //int a = *(int *)mapped;

    sleep(10);

    munmap((char *)mapped + 4096 * 3, 4096 * 3); // Страницы 4-6
    printf("Unmapped pages 4-6\n");

    sleep(20);
    return 0;
}