#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t grandchild_pid;

    pid_t child_pid = fork();

    if (child_pid == 0) {
        grandchild_pid = fork();

        if (grandchild_pid == 0) {
           
            printf("Внучатый процесс (PID: %d) работает 10 секунд...\n", getpid());
            sleep(10);
            printf("Внучатый процесс завершается.\n");
            printf("Внучатый процесс (PID: %d) работает 10 секунд...\n", getppid());
            exit(0);
        } else {
            
            printf("Дочерний процесс (PID: %d) завершается, оставляя внука.\n", getpid());
            exit(0); // Завершается, не дожидаясь внука - становится зомби
        }
    } else {
        
        printf("Родительский процесс (PID: %d) не вызывает wait().\n", getpid());
        sleep(20);
        printf("Родительский процесс завершается.\n");
    }

    return 0;
}

// watch -d -n 1 'ps -o pid,state,cmd -C zom2'