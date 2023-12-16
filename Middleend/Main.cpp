#include <stdio.h>

#include "Tree.h"
#include "TreeDump.h"
#include "ReadData.h"
#include "Diff.h"

int main() {

    FILE* input = fopen("../Frontend/Tree.txt", "r");
    MathExpression* exp = read_data(input);
    simplify_expression(exp);

    graph_dump(exp);
    fclose(input);

    return 0;
}
