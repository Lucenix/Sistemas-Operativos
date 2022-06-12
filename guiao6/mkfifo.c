#include "tralha.h"

int main(int argc, char const *argv[])
{
    if (mkfifo("fifo", 0666) == -1) {
        perror("mkfifo");
        return -1;
    }

    return 0;
}
