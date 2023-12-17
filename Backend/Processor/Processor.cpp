#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "Types.h"
#include "Logfile.h"
#include "Stack.h"
#include "Protection.h"
#include "ReadCommands.h"
#include "Processor.h"

#define DEF_CMD(name, number, argument, code) \
    case name:                                \
    code                                      \
    break;


int processor_ctor(struct Processor *proc, const char *file_name) {

    assert(proc != NULL);
    assert(file_name != NULL);

    STACK_CONSTRUCT(stk, StackDefaultCapacity);
    proc->stack = stk;

    for (int i = 0; i < NumRegs; i++) {
        proc->registers[i] = 0;
    }

    proc->output = fopen(file_name, "rb");

    return NoErrors;
}

int processor_dtor(struct Processor *proc) {

    assert(proc != NULL);

    fclose(proc->output);

    for (int i = 0; i < NumRegs; i++) {
        proc->registers[i] = 0;
    }

    stack_dtor(&proc->stack);

    return NoErrors;
}

int processor_verify(struct Processor *proc) {

    assert(proc != NULL);

    STACK_VERIFY(&proc->stack);

    if (proc == NULL) {
        fprintf(LOG_FILE, "ERROR! Pointer to processor is NULL\n\n");
        return ProcessorNull;
    }

    if (proc->output == NULL) {
        fprintf(LOG_FILE, "ERROR! Pointer to output file is NULL\n\n");
        return OutputNull;
    }

    return NoErrors;
}

int processor_dump(struct Processor *proc, const char *file, int line, const char *function, FILE* fp) {

    assert(proc != NULL);

    stack_dump(&proc->stack, file, line, function, fp);

    for (int i = 0; i < NumRegs; i++) {
        fprintf(stdout, "rax =" ELEMF "\n", proc->registers[i]);
    }
    return NoErrors;
}


int cpu(struct Processor *proc) {

    assert(proc != NULL);

    processor_verify(proc);

    int ncommands = 0;
    fread(&ncommands, sizeof(int), 1, proc->output);

    elem_t* codeArr = (elem_t*)calloc(ncommands, sizeof(elem_t));
    fread(codeArr, sizeof(elem_t), ncommands, proc->output);

    elem_t current = 0;
    elem_t ip = 0;

    while(true) {

        current = codeArr[ip];

        switch(current) {

            #include "Commands.h"

        default:
            return WrongCommand;
        }
    }
    free(codeArr);
    return NoErrors;
}


#undef DEF_CMD


int print_cat() {

    printf(" /\\_/\\ \n( o.o )\n > ^ < \n");
    return 0;
}

int print_dog() {

    printf("           __\n      (___()'`;\n      /,    /`\n      \\\\\"--\\\n\n");
    return 0;
}

int print_duck() {

    printf("    __\n___( o)>\n\\ <_. )\n `---\'\n\n");
    return 0;
}

int print_boo() {

    printf(" .-.\n(o o) boo!\n| O \\\n \\   \\\n  `~~~'\n\n");
    return 0;
}

