#include "tralha.h"

int main(int argc, char const *argv[])
{
    int fd;
    if ((fd = open("fifo", O_RDONLY)) == -1) {
        perror("open");
        return -1;
    }
    int bytes_read;
    char buffer[MAX_BUF];
    while((bytes_read = read(fd, buffer, MAX_BUF)) > 0) {
        write(1, buffer, bytes_read);
    }
    close(fd);
    return 0;
}
