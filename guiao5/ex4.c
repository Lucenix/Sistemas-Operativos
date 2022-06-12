#include "tralha.h"

int main(int argc, char const *argv[])
{
    int p[2];
    if (pipe(p) == -1) {
        perror("failed pipe");
        return -1;
    }
    if (fork() == 0) {
        close(p[0]);
        dup2(p[1], 1);
        close(p[1]);
        execlp("ls", "ls", "/etc", NULL);
        perror("failed exec");
        _exit(-1);
    } else if (fork() == 0) {
        close(p[1]);
        dup2(p[0],0);
        close(p[0]);
        execlp("wc", "wc", "-l", NULL);
        perror("failed exec");
        _exit(-1);
    } else {
        //pai, servidor
        int status;
        close(p[0]);
        close(p[1]);
        wait(&status);
        wait(&status);
    }
    return 0;
}
