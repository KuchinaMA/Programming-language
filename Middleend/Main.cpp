#include <stdio.h>

#include "Tree.h"
#include "TreeDump.h"
#include "ReadData.h"
#include "Diff.h"

int main() {

    FILE* input = fopen("../Frontend/Tree2.txt", "r");
    MathExpression* exp = read_data(input);
    simplify_expression(exp);

    graph_dump(exp);
    fclose(input);

    FILE* output = fopen("Tree2.txt", "w");
    print_tree_pre(exp, output);
    fclose(output);

    return 0;
}
