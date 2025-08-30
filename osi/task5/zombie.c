#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();

    if (pid == 0)
    {
        printf("Дочерний процесс PID: %d\n", getpid());
        exit(0);
    }
    else if (pid > 0)
    {
        printf("Родительский процесс PID: %d\n", getpid());
        printf("Создан дочерний процесс с PID: %d\n", pid);

        sleep(30); 

        wait(NULL);
    }
    else
    {
        perror("fork failed");
        return 1;
    }

    return 0;
}

// watch -d -n 1 'ps -o pid,state,cmd -C zom'
