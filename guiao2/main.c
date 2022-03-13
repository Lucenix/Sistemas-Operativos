#include "stdio.h"
#include "unistd.h"
#include "sys/wait.h"

int main(int argc, char *argv[]) {
    int pd, status;
    for(int i = 1; i<=10; i++) {
        if ((pd = fork()) == 0) {
            //printf("Filho %d: PID: %d PPID: %d\n", i, getpid(), getppid());
            _exit(i);
        }
        wait(&status);
        //if (WIFEXITED(status))
            //printf("Filho terminado: %d\n", WEXITSTATUS(status));
    }

    for(int i = 1; i<=10; i++) {
        if ((pd = fork()) == 0)
            _exit(i);
    }
    if (pd) {
        for (int i = 1; i<=10; i++) {
            wait(&status);
            printf("Filho terminado: %d\n", WEXITSTATUS(status));
        }
    }
    
}