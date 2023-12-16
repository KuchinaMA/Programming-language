#include <stdio.h>

#include "Tree.h"
#include "TreeDump.h"
#include "ReadData.h"
#include "Diff.h"

int main() {

    FILE* input = fopen("../Frontend/Tree2.txt", "r");
    MathExpression* exp = read_data(input);
    simplify_expression(exp);

    /*FILE* output = fopen("Tree.txt", "w");
    print_tree_pre(exp, output);
    fclose(output);*/

    graph_dump(exp);
    fclose(input);

    return 0;
}
