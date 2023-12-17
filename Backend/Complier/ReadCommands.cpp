#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "Types.h"
#include "Stack.h"
#include "Protection.h"
#include "ReadCommands.h"


int read_commands(FILE *fp, Stack *stk) {      //интерпретатор

    assert(fp != NULL);
    assert(stk != NULL);

    char line[MAX_LINE_LEN] = "";

    while (fscanf(fp, "%s", line) > 0) {

        if (strcmp(line, "push") == 0) {

            elem_t number = 0;
            fscanf(fp, ELEMF, &number);
            stack_push(stk, number);

            //PRINT_STACK(stk);
        }

        else if (strcmp(line, "sub") == 0) {

            elem_t second = 0;
            stack_pop(stk, &second);

            elem_t first = 0;
            stack_pop(stk, &first);

            elem_t newel = first - second;
            stack_push(stk, newel);

            //PRINT_STACK(stk);

        }

        else if (strcmp(line, "add") == 0) {

            elem_t second = 0;
            stack_pop(stk, &second);

            elem_t first = 0;
            stack_pop(stk, &first);

            elem_t newel = first + second;
            stack_push(stk, newel);

            //PRINT_STACK(stk);
        }

        else if (strcmp(line, "div") == 0) {

            elem_t second = 0;
            stack_pop(stk, &second);

            elem_t first = 0;
            stack_pop(stk, &first);

            elem_t newel = first / second;
            stack_push(stk, newel);

            //PRINT_STACK(stk);

        }

        else if (strcmp(line, "mul") == 0) {

            elem_t second = 0;
            stack_pop(stk, &second);

            elem_t first = 0;
            stack_pop(stk, &first);

            elem_t newel = first * second;
            stack_push(stk, newel);

            //PRINT_STACK(stk);
        }

        else if (strcmp(line, "out") == 0) {

            elem_t answer = 0;
            stack_pop(stk, &answer);
            printf("\n Answer: "ELEMF"\n", answer);

            //PRINT_STACK(stk);
        }

        else if (strcmp(line, "in") == 0) {

            elem_t number = 0;
            scanf(ELEMF, &number);
            stack_push(stk, number);

            //PRINT_STACK(stk);
        }

        else if (strcmp(line, "hlt") == 0) {

            return 0;
        }
    }
}


/*int complier(struct Processor *proc) {

    assert(proc != 0);

    assert(proc->input != NULL);
    assert(proc->output != NULL);

    char line[256] = "";

    while (fscanf(proc->input, "%s", line) > 0) {

        if (strcmp(line, "push") == 0) {

            fprintf(proc->output, "%d ", PUSH);

            elem_t number = 0;
            fscanf(proc->input, ELEMF, &number);

            fprintf(proc->output, ELEMF"\n", number);

        }

        else if (strcmp(line, "add") == 0)    fprintf(proc->output, "%d \n", ADD);

        else if (strcmp(line, "sub") == 0)    fprintf(proc->output, "%d \n", SUB);

        else if (strcmp(line, "mul") == 0)    fprintf(proc->output, "%d \n", MUL);

        else if (strcmp(line, "div") == 0)    fprintf(proc->output, "%d \n", DIV);

        else if (strcmp(line, "out") == 0)    fprintf(proc->output, "%d \n", OUT);

        else if (strcmp(line, "in")  == 0)    fprintf(proc->output, "%d \n", IN);

        else if (strcmp(line, "hlt") == 0)    fprintf(proc->output, "%d \n", HLT);
    }

    return NoErrors;
}
  */

/*int cpu(struct Processor *proc) {

    assert(proc != 0);

    int current = 0;

    while (fscanf(proc->output, "%d", &current) > 0) {

        switch(current) {

            case PUSH: {

                elem_t number = 0;
                fscanf(proc->output, ELEMF, &number);
                stack_push(&proc->stack, number);

                //PRINT_STACK(&proc->stack);
                break;
            }

            case ADD: {

                elem_t second = 0;
                stack_pop(&proc->stack, &second);

                elem_t first = 0;
                stack_pop(&proc->stack, &first);

                elem_t newel = first + second;
                stack_push(&proc->stack, newel);

                //PRINT_STACK(&proc->stack);
                break;
            }

            case SUB: {

                elem_t second = 0;
                stack_pop(&proc->stack, &second);

                elem_t first = 0;
                stack_pop(&proc->stack, &first);

                elem_t newel = first - second;
                stack_push(&proc->stack, newel);

                //PRINT_STACK(&proc->stack);
                break;
            }

            case MUL: {

                elem_t second = 0;
                stack_pop(&proc->stack, &second);

                elem_t first = 0;
                stack_pop(&proc->stack, &first);

                elem_t newel = first * second;
                stack_push(&proc->stack, newel);

                //PRINT_STACK(&proc->stack);
                break;
            }

            case DIV: {

                elem_t second = 0;
                stack_pop(&proc->stack, &second);

                elem_t first = 0;
                stack_pop(&proc->stack, &first);

                elem_t newel = first / second;
                stack_push(&proc->stack, newel);

                //PRINT_STACK(&proc->stack);
                break;
            }

            case OUT: {

                elem_t answer = 0;
                stack_pop(&proc->stack, &answer);
                printf("\n Answer: "ELEMF"\n", answer);

                //PRINT_STACK(&proc->stack);
                break;
            }

            case IN: {

                elem_t number = 0;
                scanf(ELEMF, &number);
                stack_push(&proc->stack, number);

                //PRINT_STACK(&proc->stack);
                break;
            }

            case HLT: {
                return NoErrors;
            }
        }
    }
} */



/*int decomplier(struct Processor *proc) {

    assert(proc != 0);

    int current = 0;

    while (fscanf(proc->output, "%d", &current) > 0) {

        switch(current) {

            case PUSH: {

                fprintf(proc->dcoutput, "push ");

                elem_t number = 0;
                fscanf(proc->output, ELEMF, &number);

                fprintf(proc->dcoutput, ELEMF"\n", number);

                break;
            }

            case ADD: fprintf(proc->dcoutput, "add\n");
                      break;


            case SUB: fprintf(proc->dcoutput, "sub\n");
                      break;


            case MUL: fprintf(proc->dcoutput, "mul\n");
                      break;


            case DIV: fprintf(proc->dcoutput, "div\n");
                      break;


            case OUT: fprintf(proc->dcoutput, "out\n");
                      break;


            case IN:  fprintf(proc->dcoutput, "in\n");
                      break;


            case HLT: fprintf(proc->dcoutput, "hlt\n");
                      break;

        }
    }
    return NoErrors;
} */








