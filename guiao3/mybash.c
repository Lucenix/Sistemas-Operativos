#include "tralha.h"
#include "malloc.h"
#include "bash.h"

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

//
int mysystemBash(char *comando) {
    int r = 0, i = 0;
    char *string;
    char *exec_argv[strlen(comando)]; //tamanho do array de argumentos é o tamanho do comando

    //tenho de fazer o teste do & antes do strtok para não os por no array do comando a fazer pelo exec
    for (string = strtok(comando, " "); string != NULL; string = strtok(NULL, " ")) {
        exec_argv[i++] = string;
    }
    //estamos a ler o '&' dentro do system o que devia ser uma coisa específica da bash
    //como fazer com que o system não dê wait de fora?
    int concurrentFlag = (!strcmp(exec_argv[i-1], "&"));
    exec_argv[i-concurrentFlag] = NULL;

    int pid;
    //o fork pode falhar
    if ((pid = fork()) == -1) {
        r = -1;
    } else if (pid == 0) {
        int ret;
        printf("Read: %s\n", exec_argv[0]);
        ret = execvp(exec_argv[0], exec_argv);
        perror("reached return");
        _exit(ret);
    } else if (!concurrentFlag) {
        //como fazer isto receber depois que @ filh@ que terminar sem fazer wait, traps? sig?
        int status;
        pid_t terminated = wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 255) {
            printf("Filh@ %d terminad@: %d\n", terminated, WEXITSTATUS(status));
            r = WEXITSTATUS(status);
        } else { //se não acabar ou se acabar com -1 
            printf("Filh@ não ou mal terminad@.\n");
            r = -1;
        }
    }
    return r;
}

int main(int argc, char const *argv[])
{
    char buffer[100];
    int bytes_read;
    while ((bytes_read = readln3(0, buffer, 100)) > 0 && strcmp(buffer, "exit\n") != 0) {
        buffer[bytes_read-1] = 0;
        mysystemBash(buffer);
        //como fazer os ifs aqui dentro para não estar sempre a mexer no mysystemBash. Não posso forçar outra fun a fazer ou não wait
    }
    return 0;
}
