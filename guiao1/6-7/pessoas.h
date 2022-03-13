
#include "string.h"
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#define Storage "sto.txt"

typedef struct {
    char nome[200];
    int idade;
}Pessoa;

int add(char *nome, char *age);
int update(int argc, char *argv[]);
int updateReg(char* reg, char* age);