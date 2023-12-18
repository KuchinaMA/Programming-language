#include <stdio.h>

#include "Tree.h"
#include "RecDescent.h"
#include "TreeDump.h"
#include "ReadData.h"
#include "Diff.h"

int main () {

    FILE* input = fopen("TestData2.txt", "rb");

    int size = size_of_file(input);
    char* line = (char*) calloc(size + 1, sizeof(char));
    fread(line, sizeof(char), size, input);

    MathExpression* exp = create_expression(line, size+1);
    graph_dump(exp);

    FILE* output = fopen("Tree2.txt", "w");
    print_tree_pre(exp, output);
    fclose(output);

    return 0;
}












