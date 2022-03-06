#include "string.h"
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#define storage "sto.txt"
#define MAX_BUF 1024*1024

typedef struct {
    int age;
    //tamanho flexivel no fim do struct huh
    char nome[];
}Pessoa;

int addPessoa(char *nome, char *age) {
    int fd;
    if ((fd = open(storage, O_WRONLY | O_CREAT, 640)) == -1) {
        perror(storage);
        return -1;
    }
    lseek(fd, 0, SEEK_END);
    char temp[strlen(nome) + strlen(age) + 2];
    
    sprintf(temp, "%s:%d\n", nome, atoi(age));
    write(fd, temp, sizeof(temp));
    close(fd);
    return 0;
}

int updatePessoa(char *nome, char *age/*trocar isto para pessoa*/) {
    //construir para dentro da struct e escrever a struct no ficheiro
    //pelos vistos funciona esta porra ^
    //mesmo se não for a struct (escrever char e depois inteiro) escrever o inteiro em inteiro kinda pog(mudar é mais facil I guess)
}

/*int updatePessoaFAILED(char *nome, char *age) {
    //quero trocar a idade 42 por 421. 
    //Como escrever em cima da antiga, escrever o novo número (sem sobrepor o \n antigo) e empurrar os bytes todos para a frente 1 casa
    //escrever sem ser em insert aka sem ser a alterar -> prob impossivel
    //eliminar a entrada e adicionar uma ao fim -> plausível mas bastante destrutiva
    //problema! ficheiro não cabe em memória -> não dá para colocar num buffer -> não dá para deslocar tudo um espaço
    //escrever inteiros resolvia o problema == refazer o exercício todo
    int fd;
    if ((fd = open(storage, O_RDWR)) == -1) {
        perror(storage);
        return -1;
    }
    lseek(fd, 0, SEEK_SET);
    int size = strlen(nome)+strlen(age)+2;
    char buffer[size+1], *n1;
    int n, flag = 1, count = 0;
    //comparar pelo tamanho não funciona, para cada entrada vou ter de olhar mesmo para o nome (idades de tamanhos diferentes)
    while (flag) {
        while((n = readln3(fd, buffer, size))>0 && n!=size) count += n;
        count += n;
        if (n == size) {
            n1 = strtok(buffer, ":");
            flag = strcmp(n1, nome);
            //char *temp = buffer;
            //for(i = 0; temp[i] != ':'; i++)
            //    n1[i] = temp[i];
        } else {
            flag = 0;
        }
    }
    if (n == 0) {return -1;}
    printf("%d, %d", count, size);
    lseek(fd, count - size, SEEK_SET);
    char temp[strlen(nome) + strlen(age) + 2];
    sprintf(temp, "%s:%s\n", nome, age);
    write(fd, temp, sizeof(temp));
    close(fd);
    return 0;
}*/

int main(int argc, char *argv[]) {
    int r = 0;
    if (argc >= 2) {
        if (!strcmp("-i", argv[1])) {
            r = addPessoa(argv[2], argv[3]);
        } else if (!strcmp("-u", argv[1])) {
            r = updatePessoa(argv[2], argv[3]);
        }
    }
    return r;
}