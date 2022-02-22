
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
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
ssize_t readln(int fd, char *line, size_t size) {
    int bytes_read, count = 0;
    while(((bytes_read = read(fd, line, 1))>0) && count+bytes_read <= size && line[0] != '\n') {
        count += bytes_read;
        line++;
    }
    line[0] == '\0';
    return count;
}

int nl(int argc, char *argv[]) {
    int fd = 0;
    if (argc >= 2 && (fd = open(argv[1], O_RDONLY)) == -1) 
        return -1;
    char buffer[MAX_BUF];
    int bytes_read, it = 1;
    while((bytes_read = readln(fd, buffer, MAX_BUF))>0) {
        char str[it];
        itoa(it, str, 10);
        strcat(str, ". ");
        write(1, str, strlen(str));
        write(1, buffer, bytes_read);
        write(1, "\n", 1);
        it++;
    }
    if (fd != 1)
        close(fd);
    return 0;
}

int main (int argc, char *argv[]) {
    nl(argc,argv);
    /*char buffer[MAX_BUF];
    int read = readln(0,buffer,4);
    printf("%d", read);
    write(1, buffer, read);*/
    return 0;
}