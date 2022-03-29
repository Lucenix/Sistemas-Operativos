//fazer dois filhos monitores para controlar a execução de cada um dos processos.

//pai que invoca os dois filhos e depois espera por um de cada vez
//tem de saber o pid dos filhos para saber quem esperar e depois quando ele acabar tem de lhe perguntar quem ele é
//depois tem de ver onde é que esse cabe dentro do array.

#include "tralha.h"

int main(int argc, char const *argv[])
{
    int i, pid;
    int count[argc-1];
    for (i = 1; i<argc; i++) {
        if ((pid = fork())==-1)
            return -1;
        else if (pid == 0) {
            //manager i
            int numb = 1, flag = 1;
            while (flag) {
                if ((pid = fork()) == -1)
                    return -1;
                else if (pid == 0) {
                    int ret;
                    ret = execl(argv[i], argv[i], NULL);
                    perror("reached return");
                    _exit(ret);
                } else {
                    int status;
                    wait(&status);
                    if (WIFEXITED(status)) {
                        if (WEXITSTATUS(status)) {
                            numb++;
                        } else {
                            flag = 0;
                        }
                    } else {
                        return -1;
                    }
                }
            }
            _exit(numb);
        } else {
            count[i-1] = pid;
        }
    }
    for (i = 1; i<argc; i++) {
        int status;
        waitpid(count[i-1], &status, 0);
        if (WIFEXITED(status)) {
            count[i-1] = WEXITSTATUS(status);
            printf("%s %d\n", argv[i], count[i-1]);
        } else {
            return -1;
        }
    }
    return 0;
}
