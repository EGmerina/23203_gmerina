#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    printf("PID: %d\n", getpid());
    sleep(1);

    char *args[] = {"./ad2a", NULL};
    execvp(args[0], args);

  
    printf("Hello world\n");  // Не выведется, так как exec заменяет процесс
    return 0;
}