#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <sys/reg.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>

const char *get_syscall_name(long syscall_num)
{
    switch (syscall_num)
    {
    case SYS_brk:
        return "brk";
    case SYS_arch_prctl:
        return "arch_prctl";
    case SYS_mmap:
        return "mmap";
    case SYS_access:
        return "access";
    case SYS_openat:
        return "openat";
    case SYS_newfstatat:
        return "newfstatat";
    case SYS_close:
        return "close";
    case SYS_read:
        return "read";
    case SYS_mprotect:
        return "mprotect";
    case SYS_pread64:
        return "pread";
    case SYS_munmap:
        return "munmap";
    case SYS_statfs:
        return "statfs";
    case SYS_set_tid_address:
        return "set_tid_address";
    case SYS_set_robust_list:
        return "set_robust_list";
    case SYS_prlimit64:
        return "prlimit64";
    case SYS_getrandom:
        return "getrandom";
    case SYS_rseq:
        return "rseq";
    case SYS_getdents64:
        return "getdents64";
    case SYS_exit_group:
        return "exit_group";
    case SYS_write:
        return "write";
    case SYS_ioctl:
        return "ioctl";
    case 5:
        return "open";
    default:
        return "unknown";
    }
}

int main(int argc, char **argv)
{

    pid_t pid = fork();
    if (pid == 0)
    {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execve("./hello", argv, NULL);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        struct user_regs_struct regs;
        while (1)
        {
            ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
                break;

            ptrace(PTRACE_GETREGS, pid, NULL, &regs);
            printf("System call: %lld, %s\n", regs.orig_rax, get_syscall_name(regs.orig_rax));
        }
    }
}
