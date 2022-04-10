
#include "guiao1.h"
#define MAX_BUF 1024*1024

int mycp(int argc, char *argv[]) {
    int fd_origin, fd_destino;
    char buffer[MAX_BUF];

    if ((fd_origin = open(argv[1], O_RDONLY)) == -1) {
        //falta os perror
        printf("Msg: %s", strerror(errno));
        return -1;
    }

    if((fd_destino = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 640)) == -1) {
        //falta os perror
        printf("Msg: %s", strerror(errno));
        return -1;
    }

    int bytes_read;
    while((bytes_read = read(fd_origin, buffer, MAX_BUF)) > 0) {
        write(fd_destino, buffer, bytes_read /* apenas escrever o que leu */);
    }

    close(fd_origin);
    close(fd_destino);
    return 0;
}

int mycat() {
    //int fd_origin, fd_destino;
    char buffer[MAX_BUF];

    //if ((fd_origin = open(argv[1], O_RDONLY)) == -1) {
    //    //falta os perror
    //    printf("Msg: %s", strerror(errno));
    //    return -1;
    //}
    //if((fd_destino = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 640)) == -1) {
    //    //falta os perror
    //    printf("Msg: %s", strerror(errno));
    //    return -1;
    //}

    int bytes_read;
    while((bytes_read = read(0, buffer, MAX_BUF)) > 0) {
        write(1, buffer, bytes_read /* apenas escrever o que leu */);
    }
    return 0;
}

//ver este read porque pode não estar a terminar a string direito? a maneira como está a escrever é muito manhosa... como ler byte a byte?
ssize_t readln1(int fd, char *line, size_t size) {
    int bytes_read, count = 0;
    //o count++ está a contar o '\n' principalmente nos casos em que só aparece '\n' no input
    //desta forma pode-se escrever count>0 apesar de nada ter sido escrito para o line sem ser o '\n'
    while(count<size && ((bytes_read = read(fd, line+count, 1))>0) && line[count++] != '\n') ;
    if (bytes_read == -1)
        return -1;
    //devolve-se o \n que facilita a vida
    return count>0&&line[count] == '\n' ? count + 1 : count;
}

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
        line[i] = '\0';
        lseek(fd, i-bytes_read, SEEK_CUR);
    }
    free(bufferLN);
    return i;
}

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


int nl(int argc, char *argv[]) {
    int fd = 0, nline = 1;
    if (argc >= 2 && (fd = open(argv[1], O_RDONLY)) == -1) 
        return -1;

    if (fd)
        lseek(fd, 0, SEEK_SET);

    //tem de se passar mais um de tamanho por causa dos \0
    char buffer[6];
    char str[32];
    int bytes_read, it = 1;

    while((bytes_read = readln2(fd, buffer, 5))>0) {
        if(nline && buffer[0] != '\n') {
            snprintf(str, 32, "%d. ", it);
            write(1, str, strlen(str));
            it++;
        }
        nline = (buffer[bytes_read - 1] == '\n');
        write(1, buffer, bytes_read);
    }

    if (fd != 0)
        close(fd);

    return 0;
}

int main (int argc, char *argv[]) {
    nl(argc,argv);
    return 0;
}