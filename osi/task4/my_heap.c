#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef struct block
{
    size_t size;
    bool is_free;
    struct block *next;
    // Данные идут сразу после структуры
} block_t;

static block_t *heap_start = NULL;
static size_t heap_size = 0;

void init_heap(size_t size)
{
    // Выравниваем размер до границы страницы
    size_t page_size = sysconf(_SC_PAGESIZE);
    heap_size = (size + page_size - 1) & ~(page_size - 1);

    // Создаем файл для mmap
    const char *filename = "file_to_test_heap.bin";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1)
    {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    // Устанавливаем размер файла
    if (ftruncate(fd, heap_size) == -1)
    {
        perror("ftruncate failed");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // heap_start = mmap(NULL, heap_size, PROT_READ | PROT_WRITE,
    //                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    heap_start = mmap(NULL, heap_size, PROT_READ | PROT_WRITE,
                      MAP_SHARED, fd, 0);
    close(fd);

    if (heap_start == MAP_FAILED)
    {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    heap_start->size = heap_size - sizeof(block_t);
    heap_start->is_free = true;
    heap_start->next = NULL;
}

void *my_malloc(size_t size)
{
    if (size == 0 || heap_start == NULL)
    {
        return NULL;
    }

    // Ищем свободный блок достаточного размера
    block_t *current = heap_start;
    block_t *best_fit = NULL;

    while (current != NULL)
    {
        if (current->is_free)
        {
            if (current->size >= size)
            {
                if (best_fit == NULL || current->size < best_fit->size)
                {
                    best_fit = current;
                }
            }
        }
        current = current->next;
    }

    if (best_fit != NULL)
    {
        if (best_fit->size > size + sizeof(block_t))
        {
            // Разделяем блок, если останется достаточно места для нового блока
            block_t *new_block = (block_t *)((char *)best_fit + sizeof(block_t) + size);
            new_block->size = best_fit->size - size - sizeof(block_t);
            new_block->is_free = true;
            new_block->next = best_fit->next;

            best_fit->size = size;
            best_fit->next = new_block;
        }

        best_fit->is_free = false;
        return (void *)(best_fit + 1); // указатель после block_t
    }

    return NULL;
}

void my_free(void *ptr)
{
    if (ptr == NULL || heap_start == NULL)
    {
        return;
    }

    block_t *block_to_free = (block_t *)ptr - 1;
    block_to_free->is_free = true;

    // Попробуем объединить соседние свободные блоки
    block_t *current = heap_start;
    while (current != NULL && current->next != NULL)
    {
        if (current->is_free && current->next->is_free)
        {
            current->size += current->next->size + sizeof(block_t);
            current->next = current->next->next;
        }
        else
        {
            current = current->next;
        }
    }
}

void heap_info()
{
    printf("Heap info:\n");
    block_t *current = heap_start;
    size_t total = 0;

    while (current != NULL)
    {
        printf("  Block at %p: size=%zu, %s\n",
               (void *)current,
               current->size,
               current->is_free ? "free" : "used");
        total += current->size + sizeof(block_t);
        current = current->next;
    }
    printf("Total allocated: %zu/%zu bytes\n", total, heap_size);
}

int main()
{

    init_heap(1024);
    sleep(3);

    printf("Initial heap:\n");
    heap_info();

    // int *arr1 = my_malloc(100 * sizeof(int));
    // sleep(3);
    // memset(arr1, 5, 10);
    // printf("\nAfter allocating arr1 (100 ints):\n");
    // heap_info();

    // char *str = my_malloc(256);
    // sleep(3);
    // printf("\nAfter allocating str (256 bytes):\n");
    // heap_info();

    // my_free(arr1);
    // sleep(3);
    // printf("\nAfter freeing arr1:\n");
    // heap_info();

    // double *arr2 = my_malloc(50 * sizeof(double));
    // sleep(3);
    // printf("\nAfter allocating arr2 (50 doubles):\n");
    // heap_info();

    // my_free(str);
    // my_free(arr2);
    // printf("\nAfter freeing all:\n");
    // heap_info();

    char *str = my_malloc(2000);
    heap_info();
    char *str1 = my_malloc(3000);
    heap_info();
    my_free(str);
    heap_info();
    str1 = my_malloc(500);
    heap_info();

    munmap(heap_start, heap_size);

    return 0;
}
// hexdump -C file_to_test_heap.bin | less
// watch -n 1 -d 'hexdump -C file_to_test_heap.bin | less'
