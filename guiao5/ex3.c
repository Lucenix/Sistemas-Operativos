#include "tralha.h"

int main(int argc, char const *argv[])
{
    int p[2];
    char buffer[20];
    if (pipe(p) == -1) {
        perror("failed pipe.");
        return -1;
    }
    switch (fork()) {
        case -1:
            perror("failed fork.");
            return -1;
        case 0:
            close(p[1]);
            dup2(p[0],0);
            close(p[0]);
            execlp("wc", "wc", NULL);
            perror("failed exec.");
            _exit(-1);
        default:
            close(p[0]);
            int status, bytes_read;
            while ((bytes_read = read(0, buffer, sizeof(buffer)))>0)
            {
                write(p[1], buffer, bytes_read);
            }
            close(p[1]);
            wait(&status);
            return 0;
    }
}
