
#include "guiao2.h"
#define Storage "sto.txt"

int main(int argc, char *argv[]) {
    int pd, status;
    for(int i = 1; i<=10; i++) {
        if ((pd = fork()) == 0) {
            printf("Filho %d: PID: %d PPID: %d\n", i, getpid(), getppid());
            _exit(i);
        }
        pid_t terminated = wait(&status);
        if (WIFEXITED(status))
            printf("Filho terminado: %d, PID: %d\n", WEXITSTATUS(status), terminated);
    }

    for(int i = 1; i<=10; i++) {
        if ((pd = fork()) == 0)
            //printf("Filho %d: PID: %d PPID: %d\n", i, getpid(), getppid());
            _exit(i);
    }
    if (pd) {
        for (int i = 1; i<=10; i++) {
            pid_t terminated = wait(&status);
            if (WIFEXITED(status))
                printf("Filho terminado: %d, PID: %d\n", WEXITSTATUS(status), terminated);
            else
                printf("Filho terminado, PID: %d\n", terminated);
        }
    }
    
    int rows = 15;
    int cols = 15000;
    int rand_max = 20000;
    int **matrix = (int **) malloc(sizeof(int*)*rows);
    int fd = open(Storage, O_WRONLY | O_CREAT);
    for (int i = 0; i< rows; i++) {
        matrix[i] = (int*) malloc(sizeof(int)*cols);
        for (int j = 0; j<cols; j++) {
            matrix[i][j] = rand() % rand_max;
        }
        if (write(fd, matrix[i], sizeof(int)*cols)<1) {
            printf("Whoopsie\n");
            return -1;
        }
    }
    
    int x = 15;
    int ar[rows];
    int i, j, r = -1;
    
    
    for (i = 0;i<rows;i++) {
        ar[i] = 0;
        if ((pd = fork()) == 0) {
            int mat[cols];
            fd = open(Storage, O_RDONLY);
            lseek(fd, sizeof(int)*cols*i, SEEK_SET);
            if (read(fd, mat, sizeof(int)*cols) < 1) {
                printf("Whoopsie\n");
                return -1;
            }
            for (j = 0; j<cols; j++) {
                if (mat[j] == x) {
                    r = i;
                    j = cols;
                }
            }
            _exit(r);
        }
    }
    for (i = 0; i<rows; i++) {
        pid_t terminated = wait(&status);
        if (WIFEXITED(status)) 
            if ((r = WEXITSTATUS(status)) != 255) {
                ar[r] = 1;
                //printf("Encontrado na linha %d, filho de pid %d\n", r, terminated);
                //tenho de esperar o wait dos outros?
            }
    }
    for (i = 0; i<rows; i++) {
        if (ar[i] == 1)
            printf("Encontrado na linha %d\n", i);
        else
            printf("Não encontrado\n");
    }
    return r;
}