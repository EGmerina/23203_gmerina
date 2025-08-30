#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int global_var = 10;

int main() {
    int local_var = 20;
    
    printf("[Parent] Global var: address=%p, value=%d\n", &global_var, global_var);
    printf("[Parent] Local var: address=%p, value=%d\n", &local_var, local_var);
    
    printf("[Parent] My PID is %d\n", getpid());
    
    pid_t child_pid = fork();
    
    if (child_pid == -1) {
        perror("fork failed");
        exit(1);
    }
    
    if (child_pid == 0) {

        printf("[Child] My PID is %d, parent PID is %d\n", getpid(), getppid());

        printf("[Child] Global var: address=%p, value=%d\n", &global_var, global_var);
        printf("[Child] Local var: address=%p, value=%d\n", &local_var, local_var);

        global_var = 100;
        local_var = 200;
        printf("[Child] Changed values: global=%d, local=%d\n", global_var, local_var);
        sleep(20);
        exit(5);
    } else {

        printf("[Parent] Values before sleep: global=%d, local=%d\n", global_var, local_var);
        
        sleep(35);

        printf("[Parent] Values after sleep: global=%d, local=%d\n", global_var, local_var);

        int status;
        pid_t terminated_pid = wait(&status);
        
        if (terminated_pid == -1) {
            perror("wait failed");
        } else {
            printf("[Parent] Child process %d terminated\n", terminated_pid);
            
            if (WIFEXITED(status)) {
                printf("[Parent] Child exited with status %d\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("[Parent] Child was killed by signal %d\n", WTERMSIG(status));
            } else if (WIFSTOPPED(status)) {
                printf("[Parent] Child was stopped by signal %d\n", WSTOPSIG(status));
            }
        }
    }
    
    return 0;
}


//watch -d -n 1 'diff /proc/6773/maps /proc/6774/maps'
//watch -d -n 1 'cat /proc/6919/maps'
//watch -d -n 1 'ps -o pid,state,cmd -C lpc'


//ps aux | grep lpc  (pid)
//watch -n 1 -d 'cat /proc/7862/smaps | grep -A 10 "heap"'   for child private_dirty!=0
