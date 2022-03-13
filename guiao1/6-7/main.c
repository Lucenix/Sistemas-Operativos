#include "pessoas.h"

int main(int argc, char *argv[]) {
    int r = 0;
    if (argc >= 2) {
        if (!strcmp("-i", argv[1])) {
            r = add(argv[2], argv[3]);
        } else if (!strcmp("-u", argv[1])) {
            r = update(argc, argv);
        } else if (!strcmp("-o", argv[1])) {
            r = updateReg(argv[2], argv[3]);
        }
    }
    return r;
}