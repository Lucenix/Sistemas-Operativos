#include "tralha.h"
#include "bash.h"

//system é bloqueante, não executa o seguinte antes de acabar
//o numero de argumentos é variável, execv para construir a lista de argumentos

int mysystem(char *comando) {
    int r = 0, i = 0;
    char *string;
    char *exec_argv[strlen(comando)]; //tamanho do array de argumentos é o tamanho do comando

    for (string = strtok(comando, " "); string != NULL; string = strtok(NULL, " ")) {
        exec_argv[i++] = string;
    }
    exec_argv[i] = NULL;

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
    } else {
        int status;
        pid_t terminated = wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 255) {
            printf("Filho %d terminado: %d\n", terminated, WEXITSTATUS(status));
            r = WEXITSTATUS(status);
        } else { //se não acabar ou se acabar com -1 
            printf("Filho não ou mal terminado.\n");
            r = -1;
        }
    }
    return r;
}

int mysystemNoWait(char *comando) {
    int r = 0, i = 0;
    char *string;
    char *exec_argv[100]; 

    for (string = strtok(comando, " "); string != NULL; string = strtok(NULL, " ")) {
        exec_argv[i++] = string;
    }
    exec_argv[i-1] = NULL;

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
    } else {
        //int status;
        //pid_t terminated = wait(&status);
        //if (WIFEXITED(status) && WEXITSTATUS(status) != 255) {
        //    printf("Filho %d terminado: %d\n", terminated, WEXITSTATUS(status));
        //    r = WEXITSTATUS(status);
        //} else { //se não acabar ou se acabar com -1 
        //    printf("Filho não ou mal terminado.\n");
        //    r = -1;
        //}
    }
    return r;
}


//int main(int argc, char const *argv[])
//{
//    char comando[] = "ls -l -a -h";
//    char comando2[] = "sleep 10";
//    char comando3[] = "ps";
//    mysystem(comando);
//    mysystem(comando2);
//    mysystem(comando3);
//
//    return 0;
//}
