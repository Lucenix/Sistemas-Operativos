
#include "malloc.h"
#include "unistd.h"
#include <fcntl.h>

#define MAX_BUF 1024

ssize_t readln3(int fd, char *line, size_t size) {
    static char bufferLN[MAX_BUF];
    static int bytesRead = 0, bytesReadMAX = 0, lastfd = 0;
    int i=0, newLineFlag = 1;
    if (lastfd != fd) {
        bytesReadMAX = 0;
        bytesRead = 0;
        lastfd = fd;
    }
    //tirar o teste da nova linha para conseguir usar o resultado neste while e não só no for
    while(newLineFlag && i<size) {
        //ler novamente apenas quando estiver tudo lido
        if (bytesRead == bytesReadMAX && (bytesReadMAX = read(lastfd, bufferLN, size-i))>0) {
            bytesRead = 0;
        } else if (bytesReadMAX == -1) return -1;
        else if (bytesReadMAX == 0) break;

        //ler até newline, buffer cheio ou tudo lido.
        for (i; newLineFlag && i < size && (bytesRead < bytesReadMAX); i++) {
            newLineFlag = bufferLN[bytesRead] != '\n';
            line[i] = bufferLN[bytesRead++];
        }
    }

    //o buffer tem +1 size para o '\0'
    line[i] = 0;
    return i;
}

int main(int argc, char const *argv[])
{
    int fdin = open("/etc/passwd", O_RDONLY);
    int fdout = open("./saida.txt", O_WRONLY | O_TRUNC | O_CREAT, 0666);
    int fderror = open("./error.txt", O_WRONLY | O_TRUNC | O_CREAT, 0666);

    int out = dup(1);
    dup2(fdin, 0);
    dup2(fdout, 1);
    dup2(fderror, 2);

    close(fdin);
    close(fdout);
    close(fderror);

    char buffer[100];
    int bytes_read;
    while((bytes_read = readln3(0, buffer, 100)) > 0) {
        write(1, buffer, bytes_read);
    }

    dup2(out, 1);
    close(out);
    write(1, "terminei\n", 20);
    return 0;
}
