#include "pessoas.h"

int add(char *nome, char *age) {
    int fd;
    if ((fd = open(Storage, O_WRONLY | O_APPEND | O_CREAT, 660)) == -1) {
        printf(strerror(errno));
        return -1;
    }
    Pessoa pessoa;
    pessoa.idade = atoi(age);
    strcpy(pessoa.nome, nome);
    if (write(fd, &pessoa, sizeof(pessoa)) < 1) {
        printf(strerror(errno));
        close(fd);
        return 1;
    }

    //struct stat st;
    //fstat(fd, &st);
    //off_t filesize = st.st_size;º
    int filesize = lseek(fd, -sizeof(Pessoa), SEEK_CUR);
    int pos = (int)filesize / sizeof(Pessoa);
    char temp[64];
    snprintf(temp, sizeof(temp), "Insertion succccessful - registo %d\n", pos);
    write(1, temp, sizeof(temp));
    close(fd);
    return 0;
}

int update(int argc, char *argv[]) {
    int fd;
    if ((fd = open(Storage, O_RDWR)) == -1) 
        return -1;
    int i = 0, bytes_read;
    Pessoa pessoa;
    while((bytes_read = read(fd, &pessoa, sizeof(pessoa)))>0) {
        if (strcmp(pessoa.nome, argv[2]) == 0) {
            pessoa.idade = atoi(argv[3]);
            lseek(fd, -sizeof(pessoa), SEEK_CUR);
            if(write(fd, &pessoa, sizeof(pessoa)) < 1) {
                close(fd);
                return 1;
            }
            char temp[64];
            snprintf(temp, sizeof(temp), "Update succccessful - registo %d\n", i);
            write(1, temp, sizeof(temp));
        }
        i++;
    }
    if (bytes_read == -1) {
        printf(strerror(errno));
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}

int updateReg(char* reg, char* age) {
    int fd;
    if ((fd = open(Storage, O_RDWR)) == -1)
        return -1;
    //struct stat st;
    //fstat(fd, &st);
    //off_t filesize = st.st_size;
    if(lseek(fd, sizeof(Pessoa)*atoi(reg), SEEK_SET)>0) {
        Pessoa pessoa;
        //lseek(fd, -sizeof(Pessoa), SEEK_CUR);
        if(read(fd, &pessoa, sizeof(Pessoa))>0) {
            pessoa.idade = atoi(age);
            lseek(fd, -sizeof(Pessoa), SEEK_CUR);
            if(write(fd, &pessoa, sizeof(Pessoa)) >= 1) {
                close(fd);
                char temp[64];
                snprintf(temp, sizeof(temp), "Update succccessful - registo %d\n", atoi(reg));
                write(1, temp, sizeof(temp));
                return 0;
            }
        }
    }
    close(fd);
    return -1;
}