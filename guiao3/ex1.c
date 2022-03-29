#include "tralha.h"

int main(int argc, char const *argv[])
{
    int ret;
    char *exec_argv[] = {"/bin/ls", "-l", NULL};

    ret = execv("/bin/ls", exec_argv);
    printf("returned %d\n", ret);
    perror("reached return");
    return 0;
}
