#include <stdio.h>

#include "Tree.h"
#include "TreeDump.h"
#include "ReadData.h"
#include "Diff.h"

int main() {

    FILE* input = fopen("../Middleend/Tree.txt", "r");
    MathExpression* exp = read_data(input);

    graph_dump(exp);
    fclose(input);

    FILE* output = fopen("Complier/AsmCode.txt", "w");
    print_tree_asm(exp, output);
    fclose(output);

    return 0;
}
