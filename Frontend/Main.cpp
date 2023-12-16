#include <stdio.h>

#include "Tree.h"
#include "RecDescent.h"
#include "TreeDump.h"
#include "ReadData.h"
#include "Diff.h"

int main () {

    FILE* input = fopen("TestData.txt", "rb");

    int size = size_of_file(input);
    char* line = (char*) calloc(size + 1, sizeof(char));
    fread(line, sizeof(char), size, input);

    MathExpression* exp = create_expression(line, size+1);
    graph_dump(exp);

    FILE* output = fopen("Tree.txt", "w");
    print_tree_pre(exp, output);
    fclose(output);

    FILE* input2 = fopen("Tree.txt", "r");
    MathExpression* exp2 = read_data(input2);
    simplify_expression(exp2);

    graph_dump(exp2);
    fclose(input2);

    return 0;
}












