#include "guiao2.h"

int matrixExistance(int matrix[][], int n, int m, int x) {
    int i, j, r = -1, pd;
    for (i = 0;i<n;i++) {
        if ((pd = fork()) == 0) {
            for (j = 0; j<m; j++) {
                if (matrix[i][j] == x) {
                    r = i;
                    j = m;
                }
            }
            _exit(r);
        }
    }

    int status;
    for (i = 0; i<n; i++) {
        pid_t terminated = wait(&status);
        if (WIFEXITED(status)) 
            if ((r = WEXITSTATUS(status)) != -1) {
                i = n;
                printf("Encontrado na linha %d, filho de pid %d\n", r, terminated);
                //tenho de esperar o wait dos outros?
            } else {
                printf("Não encontrado, filho de pid %d\n", terminated);
            }
    }
    return r;
}