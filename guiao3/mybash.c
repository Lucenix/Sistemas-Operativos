#include "tralha.h"
#include "malloc.h"
#include "bash.h"

ssize_t readln2(int fd, char *line, size_t size) {
    //a ideia é mesmo ler depois quando se chamar o readln2 again 
    //não vale a pena guardar permanentemente num buffer o que se está a ler 
    //não se sabe se o offset muda entre cada chamada de readln2 (por causa de outra função)
    //char bufferLN[size]; tem de ser com malloc
    char *bufferLN = malloc(size);
    int i;
    ssize_t bytes_read = read(fd, bufferLN, size);
    if (bytes_read == 0) return 0;
    else {
        for (i = 0; bufferLN[i] != '\n'; i++) {
            if (i >= bytes_read)
                return -1;
            line[i] = bufferLN[i];
        }
        //se não termina a linha que lê esquece, lseep
        line[i] = '\0';
        lseek(fd, i-bytes_read, SEEK_CUR);
    }
    free(bufferLN);
    return i;
}

int main(int argc, char const *argv[])
{
    char buffer[100];
    int bytes_read;
    while ((bytes_read = readln2(0, buffer, 100)) > 0 && strcmp(buffer, "exit") != 0) {
        if (buffer[bytes_read-1] == '&') {
            mysystemNoWait(buffer);
        } else {
            mysystem(buffer);
        }
        //implementação rasca do &
    }
    return 0;
}
