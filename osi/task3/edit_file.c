#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

// Функция для создания каталога
void create_directory(const char *path) {
    if (mkdir(path, 0755) == -1) {
        perror("mkdir");
    } else {
        printf("Directory created: %s\n", path);
    }
}

// Функция для вывода содержимого каталога
void list_directory(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}

// Функция для удаления каталога
void remove_directory(const char *path) {
    if (rmdir(path) == -1) {
        perror("rmdir");
    } else {
        printf("Directory removed: %s\n", path);
    }
}

// Функция для создания файла
void create_file(const char *path) {
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
    } else {
        close(fd);
        printf("File created: %s\n", path);
    }
}

// Функция для вывода содержимого файла
void print_file(const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        perror("fopen");
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }

    fclose(file);
}

// Функция для удаления файла
void remove_file(const char *path) {
    if (unlink(path) == -1) {
        perror("unlink");
    } else {
        printf("File removed: %s\n", path);
    }
}

// Функция для создания символьной ссылки
void create_symlink(const char *target, const char *linkpath) {
    if (symlink(target, linkpath) == -1) {
        perror("symlink");
    } else {
        printf("Symlink created: %s -> %s\n", linkpath, target);
    }
}

// Функция для вывода содержимого символьной ссылки
void print_symlink(const char *path) {
    char buffer[1024];
    ssize_t len = readlink(path, buffer, sizeof(buffer) - 1);
    if (len == -1) {
        perror("readlink");
    } else {
        buffer[len] = '\0';
        printf("Symlink points to: %s\n", buffer);
    }
}

// Функция для вывода содержимого файла, на который указывает символьная ссылка
void print_symlink_target(const char *path) {
    char buffer[1024];
    ssize_t len = readlink(path, buffer, sizeof(buffer) - 1);
    if (len == -1) {
        perror("readlink");
        return;
    }
    buffer[len] = '\0';
    print_file(buffer);
}

// Функция для удаления символьной ссылки
void remove_symlink(const char *path) {
    if (unlink(path) == -1) {
        perror("unlink");
    } else {
        printf("Symlink removed: %s\n", path);
    }
}

// Функция для создания жесткой ссылки
void create_hardlink(const char *target, const char *linkpath) {
    if (link(target, linkpath) == -1) {
        perror("link");
    } else {
        printf("Hardlink created: %s -> %s\n", linkpath, target);
    }
}

// Функция для удаления жесткой ссылки
void remove_hardlink(const char *path) {
    if (unlink(path) == -1) {
        perror("unlink");
    } else {
        printf("Hardlink removed: %s\n", path);
    }
}

// Функция для вывода прав доступа и количества жестких ссылок
void print_file_info(const char *path) {
    struct stat st;
    if (stat(path, &st) == -1) {
        perror("stat");
        return;
    }

    printf("File: %s\n", path);
    printf("Permissions: %o\n", st.st_mode & 0777);
    printf("Hard links: %lu\n", (unsigned long)st.st_nlink);
}

// Функция для изменения прав доступа
void change_permissions(const char *path, mode_t mode) {
    if (chmod(path, mode) == -1) {
        perror("chmod");
    } else {
        printf("Permissions changed for %s to %o\n", path, mode);
    }
}

/////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <action> <path> [additional args]\n", argv[0]);
        return 1;
    }

    // Определяем действие на основе имени программы
    char *action = strrchr(argv[0], '/');
    action = action ? action + 1 : argv[0];

    if (strcmp(action, "create_directory") == 0) {
        if (argc != 2) {
            fprintf(stderr, "Usage: %s <path>\n", action);
            return 1;
        }
        create_directory(argv[1]);
    } else if (strcmp(action, "list_directory") == 0) {
        if (argc != 2) {
            fprintf(stderr, "Usage: %s <path>\n", action);
            return 1;
        }
        list_directory(argv[1]);
    } else if (strcmp(action, "remove_directory") == 0) {
        if (argc != 2) {
            fprintf(stderr, "Usage: %s <path>\n", action);
            return 1;
        }
        remove_directory(argv[1]);
    } else if (strcmp(action, "create_file") == 0) {
        if (argc != 2) {
            fprintf(stderr, "Usage: %s <path>\n", action);
            return 1;
        }
        create_file(argv[1]);
    } else if (strcmp(action, "print_file") == 0) {
        if (argc != 2) {
            fprintf(stderr, "Usage: %s <path>\n", action);
            return 1;
        }
        print_file(argv[1]);
    } else if (strcmp(action, "remove_file") == 0) {
        if (argc != 2) {
            fprintf(stderr, "Usage: %s <path>\n", action);
            return 1;
        }
        remove_file(argv[1]);
    } else if (strcmp(action, "create_symlink") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Usage: %s <target> <linkpath>\n", action);
            return 1;
        }
        create_symlink(argv[1], argv[2]);
    } else if (strcmp(action, "print_symlink") == 0) {
        if (argc != 2) {
            fprintf(stderr, "Usage: %s <path>\n", action);
            return 1;
        }
        print_symlink(argv[1]);
    } else if (strcmp(action, "print_symlink_target") == 0) {
        if (argc != 2) {
            fprintf(stderr, "Usage: %s <path>\n", action);
            return 1;
        }
        print_symlink_target(argv[1]);
    } else if (strcmp(action, "remove_symlink") == 0) {
        if (argc != 2) {
            fprintf(stderr, "Usage: %s <path>\n", action);
            return 1;
        }
        remove_symlink(argv[1]);
    } else if (strcmp(action, "create_hardlink") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Usage: %s <target> <linkpath>\n", action);
            return 1;
        }
        create_hardlink(argv[1], argv[2]);
    } else if (strcmp(action, "remove_hardlink") == 0) {
        if (argc != 2) {
            fprintf(stderr, "Usage: %s <path>\n", action);
            return 1;
        }
        remove_hardlink(argv[1]);
    } else if (strcmp(action, "print_file_info") == 0) {
        if (argc != 2) {
            fprintf(stderr, "Usage: %s <path>\n", action);
            return 1;
        }
        print_file_info(argv[1]);
    } else if (strcmp(action, "change_permissions") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Usage: %s <path> <mode>\n", action);
            return 1;
        }
        change_permissions(argv[1], strtol(argv[2], NULL, 8));
    } else {
        fprintf(stderr, "Unknown action: %s\n", action);
        return 1;
    }

    return 0;
}