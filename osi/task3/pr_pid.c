#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PAGE_SIZE 4096  

// Функция для вывода информации о записи pagemap
void print_pagemap_entry(uint64_t entry, unsigned long address) {
  printf("Virtual Address: 0x%lx\n", address);
  printf("Page Map Entry: 0x%016lx\n", entry);
  if (entry & (1ULL << 63)) {
    printf("  Page is swapped.\n");
  } else {
    printf("  Page is in RAM.\n");
  }
  if (entry & (1ULL << 62)) {
    printf("  Page is in a page file.\n");
  }
  if (entry & (1ULL << 61)) {
    printf("  Page is exclusive to this process.\n");
  }
  if (entry & (1ULL << 55)) {
    printf("  Page is present in memory.\n");
  } else {
    printf("  Page is not present in memory.\n");
  }
  uint64_t pfn = entry & ((1ULL << 55) - 1);
  if (pfn != 0) {
    printf("  Page Frame Number: %lu\n", pfn);
  } else {
    printf("  PFN is zero (page not mapped).\n");
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
    return EXIT_FAILURE;
  }

  pid_t pid = atoi(argv[1]);

  char maps_path[256], pagemap_path[256];
  snprintf(maps_path, sizeof(maps_path), "/proc/%d/maps", pid);
  snprintf(pagemap_path, sizeof(pagemap_path), "/proc/%d/pagemap", pid);

  FILE *maps_file = fopen(maps_path, "r");
  if (!maps_file) {
    perror("Failed to open maps file");
    return EXIT_FAILURE;
  }

  int pagemap_fd = open(pagemap_path, O_RDONLY);
  if (pagemap_fd == -1) {
    perror("Failed to open pagemap file");
    fclose(maps_file);
    return EXIT_FAILURE;
  }

  char line[PAGE_SIZE];
  while (fgets(line, sizeof(line), maps_file)) {
    unsigned long start, end;
    if (sscanf(line, "%lx-%lx", &start, &end) != 2) {
      continue; 
    }

    printf("Memory range: 0x%lx - 0x%lx\n", start, end);

    for (unsigned long addr = start; addr < end; addr += PAGE_SIZE) {
      unsigned long index = addr / PAGE_SIZE;

      // Перемещаемся к нужной записи
      off_t offset = index * sizeof(uint64_t);
      if (lseek(pagemap_fd, offset, SEEK_SET) == -1) {
        perror("Failed to seek in pagemap");
        continue;
      }

      // Читаем запись из pagemap
      uint64_t entry;
      ssize_t bytes_read = read(pagemap_fd, &entry, sizeof(entry));
      if (bytes_read != sizeof(entry)) {
        perror("Failed to read pagemap entry");
        continue;
      }

      // Выводим информацию о записи
      print_pagemap_entry(entry, addr);
    }
  }

  fclose(maps_file);
  close(pagemap_fd);

  return EXIT_SUCCESS;
}
