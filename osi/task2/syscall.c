#include <unistd.h>
#include <sys/syscall.h>

void my_print()
{
    char string[] = "Hello, World!\n";
    syscall(SYS_write, 1, string, 14);
}

int main(void)
{
    my_print();
}
