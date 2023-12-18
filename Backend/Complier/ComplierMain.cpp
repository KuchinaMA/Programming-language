#include <stdio.h>

#include "Tree.h"
#include "TreeDump.h"
#include "ReadData.h"
#include "Diff.h"

#include "Types.h"
#include "Stack.h"
#include "Logfile.h"
#include "Protection.h"

#include "Complier.h"

int main() {

    FILE* input = fopen("C:/Users/admin/Documents/GitHub/Programming-language/Middleend/Tree2.txt", "r");
    MathExpression* exp = read_data(input);

    graph_dump(exp);
    fclose(input);

    FILE* output = fopen("AsmCode2.txt", "w");
    print_tree_asm(exp, output);
    fclose(output);


    FILE* input2 = fopen("AsmCode2.txt", "r");
    FILE* output2 = fopen("MachineCode2.bin", "wb");

    complier(input2, output2);

    fclose(input2);
    fclose(output2);

    printf("Compilation was successful\n");

    return 0;
}
