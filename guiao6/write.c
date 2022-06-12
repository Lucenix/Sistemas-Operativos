#include "tralha.h"

int main(int argc, char const *argv[])
{
    int fd;
    if ((fd = open("fifo", O_WRONLY)) == -1) {
        perror("open");
        return -1;
    }
    int bytes_read;
    char buffer[MAX_BUF];
    while((bytes_read = read(0, buffer, MAX_BUF)) > 0) {
        write(fd, buffer, bytes_read);
    }
    unlink(fd);
    return 0;
}
