#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

bool vacinado(char *cidadao) {
    int status;
    char regiao[16];
    pid_t pids[MAX_REGION];
    int p[MAX_REGION][2];
    for (int i = 0; i<MAX_REGION; i++) {
        pipe(p[i]);
        sprintf(regiao, "%d", i);
        pids[i] = fork();
        dup2(1, p[i][1]);
        close(p[i][1]);
        switch (pids[i])
        {
        case 0:
            execlp("grep", "grep", cidadao, regiao);
            break;
        
        default:
            break;
        }
    }

    int status, flag = 1, fd;
    for (int i = 0; i < MAX_REGION && flag; i++) {
        fd = wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            flag = 0;
        }
    }
    for (int i = 0; i<MAX_REGION; i++) {
        kill(pids[i], SIGTERM);
    }
    return !flag;
}

int vacinados(char *regiao, int idade) {
    char id[8];
    sprintf(id, " %d ", idade);
    int p[2];
    int o[2];
    pipe(p);
    pipe(o);
    dup2(1, p[1]);
    close(p[1]);
    switch (fork())
    {
    case 0: ;
        execlp("grep", "grep", id, regiao);
        break;
    
    default: ;
        dup2(0, p[0]);
        dup2(1, o[1]);
        close(p[0]);
        close(o[1]);
        switch (fork())
        {
        case 0: ;
            execlp("wc", "wc", "-l");
            break;
        
        default: ;
            int status;
            char buf[1024];
            read(o[0], buf, 1024);
            close(o[0]);
            wait(&status);
            wait(&status);
            return atoi(buf);
        }
        break;
    }
}

int main(int argc, char const *argv[])
{
    mkfifo("entry", 0666);
    int fd_i, fd_o, r = 0, w, count, bytes_read;
    fd_i = open("entry", O_RDONLY);
    fd_o = open("entry", O_WRONLY);
    char buffer[1024];
    bytes_read = read(fd_i, buffer, 1024);
    while (bytes_read>0)
    {
        char tmp[32];
        w = 0;
        count = 0;
        while (count < 2) {
            if (buffer[r] = ' ') {
                count++;
            }
            tmp[w++] = buffer[r++];
            if (r == bytes_read) {
                bytes_read = read(fd_i, buffer, 1024);
                r = 0;
            }
        }

        char file[8];
        count = 0;
        while(buffer[r] != '\0') {
            tmp[w++] = file[count++] = buffer[r++];
            if (r == bytes_read) {
                bytes_read = read(fd_i, buffer, 1024);
                r = 0;
            }
        }
        tmp[w] = buffer[r++];
        file[count] = '\n';
        if (r == bytes_read) {
            bytes_read = read(fd_i, buffer, 1024);
            r = 0;
        }
        int fd_file = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
        write(fd_file, tmp, w);

    }
    unlink("entry");
    return 0;
}
