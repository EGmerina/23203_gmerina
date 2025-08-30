#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>

#define STACK_SIZE (1024 * 1024)
#define RECURSION_DEPTH 10

static void recursive_func(int depth)
{
    if (depth <= 0)
    {
        sleep(1000);
        return;
    }

    char str[] = "  hello world";
    str[0] = '0' + depth;
    printf("Depth %d: %s\n", depth, str);

    recursive_func(depth - 1);
}

// Точка входа для нового процесса
static int child_func(void *arg)
{
    printf("Child process started\n");

    recursive_func(RECURSION_DEPTH);

    printf("Child process exiting\n");
    _exit(0);
}

int main()
{
    int fd;
    void *stack;

    //  файл для отображения стека
    fd = open("stack_file", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Увеличиваем размер файла до нужного размера стека
    if (ftruncate(fd, STACK_SIZE) == -1)
    {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE,
                 MAP_SHARED, fd, 0);
    if (stack == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    void *stack_top = (char *)stack + STACK_SIZE;

    setbuf(stdout, NULL);

    stack_top = (void *)(((uintptr_t)stack_top & ~0xF) - 8);

    pid_t pid = clone(child_func, stack_top, CLONE_VM | SIGCHLD, NULL);
    if (pid == -1)
    {
        perror("clone");
        exit(EXIT_FAILURE);
    }

    printf("Parent: child PID = %d\n", pid);
    printf("Parent: my PID = %d\n", getpid());

    sleep(1);
    munmap(stack, STACK_SIZE);
    close(fd);

    return 0;
}
// ./clone strings stack_file | grep "hello world" | wc -l
// xxd stack_file | grep "hello world"
// xxd stack_file | grep -a "0[1-9] 00 00 00"
// xxd stack_file | grep -a -E '55[0-9a-f]{7}'
// hexdump -C stack_file
