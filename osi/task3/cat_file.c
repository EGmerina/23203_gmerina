#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define PAGE_SIZE 4096 // Размер страницы памяти

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
        return 1;
    }

    // Формируем путь к файлу pagemap
    char pagemap_path[256];
    snprintf(pagemap_path, sizeof(pagemap_path), "/proc/%s/pagemap", argv[1]);

    // Открываем файл pagemap
    int pagemap_fd = open(pagemap_path, O_RDONLY);
    if (pagemap_fd == -1)
    {
        fprintf(stderr, "Failed to open %s: %s\n", pagemap_path, strerror(errno));
        return 1;
    }

    // Читаем записи из pagemap
    uint64_t entry;
    off_t offset = 0; 

    while (read(pagemap_fd, &entry, 8) > 0)
    {
        // Выводим информацию о странице
        printf("Page at offset 0x%lx:\n", offset);
        printf("  Present: %ld\n", (entry >> 63) & 1);
        printf("  Swapped: %ld\n", (entry >> 62) & 1);
        printf("  File page: %ld\n", (entry >> 61) & 1);
        printf("  Physical address: 0x%lx\n", (entry & 0x7FFFFFFFFFFFFF) * PAGE_SIZE);
        printf("  Page: 0x%lx\n", entry);

        offset += PAGE_SIZE;
    }

    // Закрываем файл
    close(pagemap_fd);

    return 0;
}