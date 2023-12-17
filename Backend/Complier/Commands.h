
DEF_CMD(PUSH, 1, 1, {
    elem_t number = codeArr[ip+1];
    ip++;
    stack_push(&proc->stack, number);
    ip++;
    })

DEF_CMD(PUSH_R, 2, 2, {
    elem_t number = codeArr[ip+1];
    if (number == RAX || number == RBX || number == RCX) {
        stack_push(&proc->stack, proc->registers[number]);
        ip++;
    }
    else
        printf("Incorrect register\n");
    ip++;
    })

DEF_CMD(POP, 3, 2, {
    elem_t number = codeArr[ip+1];
    if (number == RAX || number == RBX || number == RCX) {
        stack_pop(&proc->stack, &proc->registers[number]);
        ip++;
    }
    else
        printf("Incorrect register\n");
    ip++;
    })

DEF_CMD(IN, 4, 0, {
    elem_t number = 0;
    scanf(ELEMF, &number);
    stack_push(&proc->stack, number);
    ip++;
    })

DEF_CMD(OUT, 5, 0, {
    elem_t answer = 0;
    stack_pop(&proc->stack, &answer);
    printf("\n Answer: " ELEMF "\n", answer);
    ip++;
    })


DEF_CMD(RET, 6, 0, {
    elem_t address = 0;
    stack_pop(&proc->stack, &address);
    ip = address;
    })

//DEF_CMD(CALL, 7, 0, {



#define DEF_ARITHM(name, numb, operation)  \
DEF_CMD(name, numb, 0, {                   \
    elem_t second = 0;                     \
    stack_pop(&proc->stack, &second);      \
                                           \
    elem_t first = 0;                      \
    stack_pop(&proc->stack, &first);       \
                                           \
    elem_t newel = first operation second; \
    stack_push(&proc->stack, newel);       \
    ip++;                                  \
    })

DEF_ARITHM(ADD, 10, +)
DEF_ARITHM(SUB, 11, -)
DEF_ARITHM(MUL, 12, *)
DEF_ARITHM(DIV, 13, /)

#undef DEF_ARITHM


DEF_CMD(SQRT, 14, 0, {
    elem_t number = 0;
    stack_pop(&proc->stack, &number);

    elem_t newel = sqrt(number);
    stack_push(&proc->stack, newel);
    ip++;
    })

DEF_CMD(POW, 15, 0, {
    elem_t second = 0;
    stack_pop(&proc->stack, &second);

    elem_t first = 0;
    stack_pop(&proc->stack, &first);

    elem_t newel = pow(first,second);
    stack_push(&proc->stack, newel);
    ip++;
    })


DEF_CMD(SIN, 20, 0, {
    elem_t number = 0;
    stack_pop(&proc->stack, &number);

    elem_t newel = sin(number);
    stack_push(&proc->stack, newel);
    ip++;
    })

DEF_CMD(COS, 21, 0, {elem_t number = 0;
    stack_pop(&proc->stack, &number);

    elem_t newel = cos(number);
    stack_push(&proc->stack, newel);
    ip++;
    })

DEF_CMD(TAN, 22, 0, {
    elem_t number = 0;
    stack_pop(&proc->stack, &number);

    elem_t newel = tan(number);
    stack_push(&proc->stack, newel);
    ip++;
    })

DEF_CMD(CTG, 23, 0, {
    elem_t number = 0;
    stack_pop(&proc->stack, &number);

    elem_t newel = 1 / tan(number);
    stack_push(&proc->stack, newel);
    ip++;
    })


DEF_CMD(MEOW, 30, 0, {
    print_cat();
    ip++;
    })

DEF_CMD(BARK, 31, 0, {
    print_dog();
    ip++;
    })

DEF_CMD(DUCK, 32, 0, {
    print_duck();
    ip++;
    })

DEF_CMD(BOO, 33, 0, {
    print_boo();
    ip++;
    })


DEF_CMD(SOLVE, 40, 0, {
    printf("I can count the discriminant!\n");
    ip++;
    })

DEF_CMD(SLEEP, 41, 0, {
    printf("Stop boting, it's time to sleep\n");
    ip++;
    })

DEF_CMD(MATAN, 42, 0, {
    printf("There's no tine to sleep, you need to botat' matan\n");
    ip++;
    })


DEF_CMD(HLT, -1, 0, {
    return NoErrors;
    })


DEF_CMD(JMP, 50, 3, {
    ip = codeArr[ip+1];
    })


#define DEF_CONDITIONAL_JMP(name, numb, operation) \
DEF_CMD(name, numb, 3, {                           \
    elem_t lim = 0;                                \
    stack_pop(&proc->stack, &lim);                 \
    elem_t cur = 0;                                \
    stack_pop(&proc->stack, &cur);                 \
    if (cur operation lim) {                       \
        ip = codeArr[ip+1];                        \
    }                                              \
    else ip += 2;                                  \
    })

DEF_CONDITIONAL_JMP(JB, 51, <)
DEF_CONDITIONAL_JMP(JBE, 52, <=)
DEF_CONDITIONAL_JMP(JA, 53, >)
DEF_CONDITIONAL_JMP(JAE, 54, <=)
DEF_CONDITIONAL_JMP(JE, 55, ==)
DEF_CONDITIONAL_JMP(JNE, 56, !=)

#undef DEF_CONDITIONAL_JMP





