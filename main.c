#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <sys/user.h>

#define XOR_KEY 233
#define SYS_write_XOR (SYS_write^XOR_KEY)

const char msg_fuckup[] = "fuck me up plz\n";

void tracee();

void tracer();

int main() {
    printf("Weird Linux Anti-debugging Demo\n");
    pid_t child_pid = fork();
    if (child_pid < 0) {
        printf("Fork failed.\n");
        return 1;
    }
    if (child_pid == 0) {
        tracee(); //child
    } else {
        tracer(child_pid);
    }
}

void tracee() {
    ptrace(PTRACE_TRACEME, 0, 0, 0);
    syscall(SYS_write_XOR, 1, msg_fuckup, sizeof(msg_fuckup) - 1);
    syscall(SYS_write_XOR, 1, msg_fuckup, sizeof(msg_fuckup) - 1);
    syscall(SYS_write_XOR, 1, msg_fuckup, sizeof(msg_fuckup) - 1);
}

void tracer(pid_t child_pid) {
    waitpid(child_pid, 0, 0);
    ptrace(PTRACE_SETOPTIONS, child_pid, 0, PTRACE_O_EXITKILL);
    struct user_regs_struct regs;
    while (1) {
        ptrace(PTRACE_SYSCALL, child_pid, 0, 0);
        waitpid(child_pid, 0, 0);
        ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
        regs.orig_rax ^= XOR_KEY;
        ptrace(PTRACE_SETREGS, child_pid, 0, &regs);
    }
}