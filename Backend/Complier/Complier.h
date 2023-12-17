#ifndef COMPLIER_H_INCLUDED
#define COMPLIER_H_INCLUDED

#include <stdio.h>

const int ComplierVersion = 4;
static const char *Signature = "mipt";
const int LABELS_NUM = 10;

enum ComplierErrors {
    IncorrectInput   = 1,
    IncorrectOutput  = 1 << 1,
    IncorrectCommand = 1 << 2,
    InvalidVersion   = 1 << 3,
    InvalidSignature = 1 << 4
};

int complier(FILE* input, FILE* output);
int print_reg(char* line, elem_t* codeArr, int position);
int check_file(FILE* input, FILE* output);

#endif // COMPLIER_H_INCLUDED
