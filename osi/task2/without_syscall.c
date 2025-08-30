#include <unistd.h>

int main()
{
    char string[] = "Hello, World!\n";
    size_t length = 14; 

    __asm__ volatile(
        "mov $1, %%rax\n" // Номер системного вызова write (1 для x86_64)
        "mov $1, %%rdi\n" // Файловый дескриптор stdout (1)
        "mov %0, %%rsi\n" // Указатель на строку (message)
        "mov %1, %%rdx\n" // Длина строки (length)
        "syscall"         // Вызов системного вызова
        :
        : "r"(string), "r"(length)
        : "%rax", "%rdi", "%rsi", "%rdx");

    return 0;
}