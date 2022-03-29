#include "tralha.h"

int main(int argc, char const *argv[])
{
    int pid;
    pid = fork();
    if (pid == 0) {
        int ret;
        char *exec_argv[] = {"/bin/ls", "-l", NULL};        
        ret = execv("/bin/ls", exec_argv);
        perror("reached return");
        _exit(ret);
    } else {
        int status;
        wait(&status);
        if (WIFEXITED(status)) {
            printf("Filho retornou: %d\n", WEXITSTATUS(status));
        } else {
            printf("Filho não retornou\n");
        }
    }

    return 0;
}
