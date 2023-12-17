#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

#include <stdio.h>

enum ProcessorErrors {
    ProcessorNull = 1 << 7,
    OutputNull    = 1 << 8,
    WrongCommand  = 1 << 9
};


const int ProcessorVersion = 2;
const int NumRegs = 3;

struct Processor {
    struct Stack stack;
    struct Stack addresses;
    elem_t registers[NumRegs];
    FILE* output;
};

int processor_ctor(struct Processor *proc, const char *file_name);
int processor_dtor(struct Processor *proc);
int processor_verify(struct Processor *proc);
int processor_dump(struct Processor *proc, const char *file, int line, const char *function, FILE* fp);

int cpu(struct Processor *proc);

int print_cat();
int print_dog();
int print_duck();
int print_boo();

#endif // PROCESSOR_H_INCLUDED
