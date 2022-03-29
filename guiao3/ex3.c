#include "tralha.h"

int main(int argc, char const *argv[])
{
    int i, pid;
    for (i = 1; i<argc; i++) {
        pid = fork();
        if (pid == 0) {
            int exec_ret;
            printf("Filho %d, comando %s\n", getpid(), argv[i]);
            exec_ret = execlp(argv[i], argv[i], NULL);
            perror("reached return");
            _exit(exec_ret);
        }
    }

    for (i = 1; i<argc; i++) {
        printf("À espera dos filhos\n");
        wait(NULL);
    }

    printf("Todos os filhos executaram\n");
    return 0;
}
