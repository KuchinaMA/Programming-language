#ifndef READCOMMANDS_H_INCLUDED
#define READCOMMANDS_H_INCLUDED

#define DEF_CMD(name, number, args, code) \
    name = number,

#include <stdio.h>

const int MAX_LINE_LEN = 256;
const int MAX_ARR_LEN = 200;


enum Commands {
    #include "Commands.h"
};

#undef DEF_CMD

enum Regs {
    RAX = 0,
    RBX = 1,
    RCX = 2,
    RDX = 3,
    R1X = 4,
    R2X = 5
};

enum TypeArgs {
    NumArg = 1,
    RegArg = 2,
    JmpArg = 3
};



#endif // READCOMMANDS_H_INCLUDED
