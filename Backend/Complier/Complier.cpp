#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Types.h"
#include "Stack.h"
#include "Protection.h"
#include "ReadCommands.h"
#include "Complier.h"


#define DEF_CMD(name, number, argument, code)    \
    if (strcmp(line, #name) == 0) {              \
        codeArr[position++] = name;              \
        if (argument == NumArg) {                \
        elem_t num = 0;                          \
            fscanf(input, ELEMF, &num);          \
            codeArr[position++] = num;           \
        }                                        \
        else if (argument == RegArg) {           \
            char reg[MAX_LINE_LEN] = "";         \
            fscanf(input, "%s", reg);            \
            print_reg(reg, codeArr, position++); \
        }                                        \
        else if (argument == JmpArg) {           \
            char label[MAX_LINE_LEN] = "";       \
            fscanf(input, "%s", label);          \
            int adress = label[1] - '0';         \
            codeArr[position++] = labels[adress];\
        }                                        \
    }                                            \
    else

            /*int label = 0;                       \
            fgetc();                             \
            fscanf(input, "%d", label);          \
            codeArr[position++] = labels[label]; \ */


int complier(FILE* input, FILE* output) {

    assert(input != NULL);
    assert(output != NULL);

    check_file(input, output);

    elem_t *codeArr = (elem_t*)calloc(MAX_ARR_LEN, sizeof(elem_t));
    int position = 0;

    int labels[LABELS_NUM] = {};
    for (int i = 0; i < LABELS_NUM; i++)
        labels[i] = -1;

    char line[MAX_LINE_LEN] = "";

    while (fscanf(input, "%s", line) > 0) {

        if (line[0] == ':')  {
            labels[line[1] - '0'] = position;
        }

        else {

            #include "Commands.h"

            /*else*/ {

                printf("Compilation failed: incorrect command\n");
                return IncorrectCommand;
            }
        }
    }


    position = 0;
    fseek(input, 9, SEEK_SET);        //пропускаем сигнатуру и версию и сканируем файл ещё раз

    while (fscanf(input, "%s", line) > 0) {

        if (line[0] == ':')  {
            labels[line[1] - '0'] = position;
        }

        else {

            #include "Commands.h"

            /*else*/ {

                printf("Compilation failed: incorrect command\n");
                return IncorrectCommand;
            }
        }
    }

    fwrite(&position, sizeof(int), 1, output);
    fwrite(codeArr, sizeof(elem_t), position, output);

    free(codeArr);

    return NoErrors;
}

#undef DEF_CMD


int check_file(FILE* input, FILE* output) {

    assert(input != NULL);
    assert(output != NULL);

    int version = 0;
    fscanf(input, "%d", &version);

    char sign[MAX_LINE_LEN] = "";
    fscanf(input, "%s", sign);


    if (version != ComplierVersion) {
        printf("Сompiler and data versions do not match\n");
        return InvalidVersion;
    }

    else if (strcmp(sign, Signature)) {
        printf("Incorrect signature\n");
        return InvalidSignature;
    }

    else {

        return NoErrors;
    }
}


int print_reg(char* line, elem_t* codeArr, int position) {

    assert(line != NULL);
    assert(codeArr != NULL);
    assert(position >= 0);

    if (strcmp(line, "rax") == 0)      codeArr[position] = (elem_t)RAX;

    else if (strcmp(line, "rbx") == 0) codeArr[position] = (elem_t)RBX;

    else if (strcmp(line, "rcx") == 0) codeArr[position] = (elem_t)RCX;

    else if (strcmp(line, "rdx") == 0) codeArr[position] = (elem_t)RDX;

    else printf("Incorrect register");

    return NoErrors;
}


