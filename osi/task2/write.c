#include <unistd.h>

int main(void)
{
    char string[] = "Hello, World!\n";
    write(1, string, 14);
}
