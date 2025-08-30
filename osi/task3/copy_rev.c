#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Функция для реверсирования строки
void reverse_string(char *str)
{
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++)
    {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

// Функция для реверсирования содержимого файла
void reverse_file_content(const char *src_path, const char *dst_path)
{
    int src_fd = open(src_path, O_RDONLY);
    if (src_fd == -1)
    {
        perror("open src");
        return;
    }

    // Получаем размер файла
    struct stat st;
    if (fstat(src_fd, &st) == -1)
    {
        perror("fstat");
        close(src_fd);
        return;
    }
    off_t file_size = st.st_size;

    // Читаем содержимое файла
    char *buffer = malloc(file_size);
    if (read(src_fd, buffer, file_size) != file_size)
    {
        perror("read");
        free(buffer);
        close(src_fd);
        return;
    }
    close(src_fd);

    // Реверсируем содержимое
    for (off_t i = 0; i < file_size / 2; i++)
    {
        char temp = buffer[i];
        buffer[i] = buffer[file_size - i - 1];
        buffer[file_size - i - 1] = temp;
    }

    // Записываем реверсированное содержимое в новый файл
    int dst_fd = open(dst_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst_fd == -1)
    {
        perror("open dst");
        free(buffer);
        return;
    }
    if (write(dst_fd, buffer, file_size) != file_size)
    {
        perror("write");
    }
    close(dst_fd);
    free(buffer);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    char *src_dir = argv[1];
    char dst_dir[256];

    // Реверсируем имя каталога
    strncpy(dst_dir, src_dir, sizeof(dst_dir));
    reverse_string(dst_dir);

    // Создаем новый каталог
    mkdir(dst_dir, 0755);

    // Открываем исходный каталог
    DIR *dir = opendir(src_dir);
    if (!dir)
    {
        perror("opendir");
        return 1;
    }

    // Обходим все файлы в каталоге
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        // Проверяем, что это регулярный файл
        if (entry->d_type != DT_REG)
        {
            continue;
        }
        // Проверяем, что это регулярный файл
        char src_path[512];
        char dst_path[512];

        // Формируем полные пути к исходному и целевому файлам
        snprintf(src_path, sizeof(src_path), "%s/%s", src_dir, entry->d_name);
        snprintf(dst_path, sizeof(dst_path), "%s/", dst_dir);

        // Реверсируем имя файла
        char reversed_name[256];
        strncpy(reversed_name, entry->d_name, sizeof(reversed_name));
        reverse_string(reversed_name);
        strncat(dst_path, reversed_name, sizeof(dst_path) - strlen(dst_path) - 1);

        // Копируем файл с реверсированным содержимым
        reverse_file_content(src_path, dst_path);
    }
    closedir(dir);
    return 0;
}