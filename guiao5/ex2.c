#include "tralha.h"

int main(int argc, char const *argv[])
{
    int p[2], bytes_read;
    char buffer[100];
    if (pipe(p) == -1) {
        perror("failed pipe.");
        return -1;
    }
    switch (fork())
    {
    case 0:
        close(p[1]);
        while((bytes_read = read(p[0], buffer, sizeof(buffer))) > 0) {
            write(1, buffer, bytes_read);
        }
        close(p[0]);
        _exit(0);
        break;
    
    default:
        close(p[0]);
        int status;
        
        while((bytes_read = read(0, buffer, sizeof(buffer)) > 0)) {
            write(p[1], buffer, bytes_read);
        }
        close(p[1]);
        wait(&status);
        break;
    }
    return 0;
}
