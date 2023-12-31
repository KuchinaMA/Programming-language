#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "Tree.h"
#include "TreeDump.h"
#include "ReadData.h"
#include "Diff.h"

void print_data(const Node* node, FILE* output) {

    switch (node->type) {
        case T_NUM:
            fprintf(output, " %d", node->data);
            break;
        case T_OP:
            fprintf(output, " %s", operation_to_sign(node));
            break;
    }
}

void print_tree_pre(const MathExpression* expression, FILE* output) {
    print_node_pre(expression, expression->tree->root, output, BEGIN_OP, MID_POS);
}

void print_node_pre(const MathExpression* expression, const Node* node, FILE* output, int parent_data, int position) {

    if (node == 0) {
        fprintf(output, " nil");
        return;
    }

    if (node->type == T_NUM)
        print_num(expression, node, output);
    else if (node->type == T_OP)
        print_oper(expression, node, output, parent_data, position);

    else if (node->type == T_VAR)
        print_var(expression, node, output);
}

void print_num(const MathExpression* expression, const Node* node, FILE* output) {

    fprintf(output, " (");
    fprintf(output, " %d", node->data);

    print_node_pre(expression, node->left, output, node->data, LEFT_POS);
    print_node_pre(expression, node->right, output, node->data, RIGHT_POS);

    fprintf(output, " )");
}

void print_oper(const MathExpression* expression, const Node* node, FILE* output, int parent_data, int position) {

    fprintf(output, " (");
    fprintf(output, " %s", operation_to_sign(node));

    print_node_pre(expression, node->left, output, node->data, LEFT_POS);
    print_node_pre(expression, node->right, output, node->data, RIGHT_POS);

    fprintf(output, " )");
}

void print_var(const MathExpression* expression, const Node* node, FILE* output) {

    fprintf(output, " (");
    fprintf(output, " %s", expression->variables_table[node->data].name);

    print_node_pre(expression, node->left, output, node->data, LEFT_POS);
    print_node_pre(expression, node->right, output, node->data, RIGHT_POS);

    fprintf(output, " )");
}


char* operation_to_sign(const Node* node) {

    switch (node->data) {
        case ADD:
            return "add";
        case SUB:
            return "sub";
        case MUL:
            return "*";
        case DIV:
            return "/";
        case LN:
            return "ln";
        case POW:
            return "^";
        case SIN:
            return "sin";
        case COS:
            return "cos";
        case TAN:
            return "tan";
        case CTG:
            return "ctg";
        case EQUAL:
            return "=";
        case MORE:
            return "more";
        case LESS:
            return "less";
        case IF:
            return "if";
        case WHILE:
            return "while";
        case IN:
            return "in";
        case SEP:
            return ";";
    }
}


char* operation_to_sign_graph(const Node* node) {

    switch (node->data) {
        case ADD:
            return "+";
        case SUB:
            return "-";
        case MUL:
            return "*";
        case DIV:
            return "/";
        case LN:
            return "ln";
        case POW:
            return "^";
        case SIN:
            return "sin";
        case COS:
            return "cos";
        case TAN:
            return "tan";
        case CTG:
            return "ctg";
        case EQUAL:
            return "=";
        case MORE:
            return "more";
        case LESS:
            return "less";
        case IF:
            return "if";
        case WHILE:
            return "while";
        case IN:
            return "in";
        case SEP:
            return ";";
    }
}


int compare_operations(int parent_op, int cur_op, int position) {

    if (parent_op == ADD)
        return 0;

    else if (parent_op == SUB) {
        if (cur_op == ADD || cur_op == SUB) {
            if (position == RIGHT_POS)
                return 1;
        }
        else
            return 0;
    }

    else if (parent_op == MUL) {
        if (cur_op == MUL || cur_op == DIV)
            return 0;

        else
            return 1;
    }

    else if (parent_op == DIV) {
        if (cur_op == MUL || cur_op == DIV) {
            if (position == LEFT_POS)
                return 0;
        }
        else
            return 1;
    }

    else if (parent_op == BEGIN_OP)
        return 0;
}


int graph_dump(const MathExpression* expression) {

    FILE* dotfile = fopen("TreePicture.dot", "w");

    fprintf(dotfile, "digraph {\n");
    fprintf(dotfile, "  rankdir = HR;\n");
    fprintf(dotfile, "  node [shape = Mrecord, color = \"#660066\", style = filled, fillcolor = \"#DFBFFF\"];\n");

    node_graph_dump(expression, expression->tree->root, dotfile);
    edge_graph_dump(expression->tree->root, dotfile);

    fprintf(dotfile, "}");

    fclose(dotfile);

    system("dot TreePicture.dot -T png -o TreePicture.png");

    return 0;
}


void node_graph_dump(const MathExpression* expression, const Node* node, FILE* dotfile) {

    if (node == 0)
        return;
    if (node->type == T_NUM)
        fprintf(dotfile, "  node_%p[label = \" %d \"]; \n", node, node->data);
    else if (node->type == T_OP) {
        if (node->data == SEP)
            fprintf(dotfile, "  node_%p[label = \" %s \", color = \"#FF9933\", style = filled, fillcolor = \"#FFFFBF\"]; \n",
                            node, operation_to_sign_graph(node));
        else
            fprintf(dotfile, "  node_%p[label = \" %s \", color = \"#000066\", style = filled, fillcolor = \"#D5EAFF\"]; \n",
                            node, operation_to_sign_graph(node));
    }
    else if (node->type == T_VAR)
        fprintf(dotfile, "  node_%p[label = \" %s \", color = \"#660033\", style = filled, fillcolor = \"#FFD5EA\"]; \n",
                            node, expression->variables_table[node->data].name);
    node_graph_dump(expression, node->left, dotfile);
    node_graph_dump(expression, node->right, dotfile);

}


void edge_graph_dump(const Node* node, FILE* dotfile) {

    if (node->left != 0) {
        fprintf(dotfile, "  node_%p -> node_%p [weight = 1];\n", node, node->left);
        edge_graph_dump(node->left, dotfile);
    }

    if (node->right != 0) {
        fprintf(dotfile, "  node_%p -> node_%p [weight = 1];\n", node, node->right);
        edge_graph_dump(node->right, dotfile);
    }
}



