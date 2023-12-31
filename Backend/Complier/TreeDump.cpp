#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "Tree.h"
#include "TreeDump.h"
#include "ReadData.h"
#include "Diff.h"

static int LabelNum = 1;


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

    //if (node == 0)
        //return;

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
    //fprintf(output, " %s", operation_to_sign(node));
    print_node_pre(expression, node->right, output, node->data, RIGHT_POS);
    fprintf(output, " )");
}

void print_var(const MathExpression* expression, const Node* node, FILE* output) {

    fprintf(output, " (");
    fprintf(output, " %s", expression->variables_table[node->data].name);
    print_node_pre(expression, node->left, output, node->data, LEFT_POS);
    //fprintf(output, " %s", expression->variables_table[node->data].name);
    print_node_pre(expression, node->right, output, node->data, RIGHT_POS);
    fprintf(output, " )");
}


char* operation_to_sign(const Node* node) {

    switch (node->data) {
        case ADD:
            return "ADD";
        case SUB:
            return "SUB";
        case MUL:
            return "MUL";
        case DIV:
            return "DIV";
        case LN:
            return "LN";
        case POW:
            return "POW";
        case SIN:
            return "SIN";
        case COS:
            return "COS";
        case TAN:
            return "TAN";
        case CTG:
            return "CTG";
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
    //fprintf(dotfile, "-Gcharset = \"UTF-8\";\n");
    fprintf(dotfile, "  rankdir = HR;\n");
    fprintf(dotfile, "  node [shape = Mrecord, color = \"#660066\", style = filled, fillcolor = \"#DFBFFF\"];\n");

    node_graph_dump(expression, expression->tree->root, dotfile);
    edge_graph_dump(expression->tree->root, dotfile);

    fprintf(dotfile, "}");

    fclose(dotfile);

    //system("chcp 65001");
    system("dot TreePicture.dot -T png -o TreePicture.png");
    //system("chcp 1252");

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


void print_tree_asm(const MathExpression* expression, FILE* output) {
    fprintf(output, "4\n");
    fprintf(output, "mipt\n");
    print_node_asm(expression, expression->tree->root, output, BEGIN_OP, MID_POS);
    fprintf(output, "PUSH_R r1x\n");
    fprintf(output, "OUT\n");
    fprintf(output, "PUSH_R r2x\n");
    fprintf(output, "OUT\n");
    //fprintf(output, "PUSH_R rax\n");
    //fprintf(output, "OUT\n");
    fprintf(output, "HLT\n");
}

void print_node_asm(const MathExpression* expression, const Node* node, FILE* output, int parent_data, int position) {

    if (node == 0)
        return;

    if (node->type == T_NUM)
        print_num_asm(expression, node, output);
    else if (node->type == T_OP)
        print_oper_asm(expression, node, output, parent_data, position);

    else if (node->type == T_VAR)
        print_var_asm(expression, node, output);
}

void print_num_asm(const MathExpression* expression, const Node* node, FILE* output) {

    fprintf(output, "%d\n", node->data);
}

void print_oper_asm(const MathExpression* expression, const Node* node, FILE* output, int parent_data, int position) {

    if (ADD <= node->data && node->data <= DIV) {

        if(node->left->type == T_NUM)
            fprintf(output, "PUSH ");
        else if(node->left->type == T_VAR)
            fprintf(output, "PUSH_R ");
        print_node_asm(expression, node->left, output, node->data, LEFT_POS);

        if(node->right->type == T_NUM)
            fprintf(output, "PUSH ");
        else if(node->right->type == T_VAR)
            fprintf(output, "PUSH_R ");
        print_node_asm(expression, node->right, output, node->data, RIGHT_POS);

        fprintf(output, "%s\n", operation_to_sign(node));
    }

    else if (node->data == POW) {

        if (node->right->data == DIV && node->right->left->type  == T_NUM && node->right->left->data  == 1
                                     && node->right->right->type == T_NUM && node->right->right->data == 2) {

            if(node->left->type == T_NUM)
                fprintf(output, "PUSH ");
            else if(node->left->type == T_VAR)
                fprintf(output, "PUSH_R ");
            print_node_asm(expression, node->left, output, node->data, LEFT_POS);

            fprintf(output, "SQRT\n");
        }

        else {

            if(node->left->type == T_NUM)
                fprintf(output, "PUSH ");
            else if(node->left->type == T_VAR)
                fprintf(output, "PUSH_R ");
            print_node_asm(expression, node->left, output, node->data, LEFT_POS);

            if(node->right->type == T_NUM)
                fprintf(output, "PUSH ");
            else if(node->right->type == T_VAR)
                fprintf(output, "PUSH_R ");
            print_node_asm(expression, node->right, output, node->data, RIGHT_POS);

            fprintf(output, "%s\n", operation_to_sign(node));

        }

    }

    else if (node->data == WHILE) {

        fprintf(output, ":%d\n", LabelNum);
        //printf("here\n");
        int old_label = LabelNum;
        LabelNum++;

        print_node_asm(expression, node->left, output, node->data, LEFT_POS);
        print_node_asm(expression, node->right, output, node->data, RIGHT_POS);

        fprintf(output, "JMP :%d\n", old_label);
        fprintf(output, ":%d\n", LabelNum);
        LabelNum++;

    }

    else if (node->data == IF) {

        print_node_asm(expression, node->left, output, node->data, LEFT_POS);
        print_node_asm(expression, node->right, output, node->data, RIGHT_POS);

        fprintf(output, ":%d\n", LabelNum);
        LabelNum++;

    }

    else if (node->data == MORE || node->data == LESS) {

        if(node->left->type == T_NUM)
            fprintf(output, "PUSH ");
        else if(node->left->type == T_VAR)
            fprintf(output, "PUSH_R ");
        print_node_asm(expression, node->left, output, node->data, LEFT_POS);

        if(node->right->type == T_NUM)
            fprintf(output, "PUSH ");
        else if(node->right->type == T_VAR)
            fprintf(output, "PUSH_R ");
        print_node_asm(expression, node->right, output, node->data, RIGHT_POS);

        if (node->data == LESS)
            fprintf(output, "JA :%d\n", LabelNum);

        else if (node->data == MORE)
            fprintf(output, "JB :%d\n", LabelNum);
    }

    else if (node->data == EQUAL) {

        if (node->right->type == T_OP)
            print_node_asm(expression, node->right, output, node->data, RIGHT_POS);
        else if (node->right->type == T_NUM) {
            fprintf(output, "PUSH ");
            print_node_asm(expression, node->right, output, node->data, RIGHT_POS);
        }

        fprintf(output, "POP ");
        print_node_asm(expression, node->left, output, node->data, LEFT_POS);

    }

    else if (node->data == SEP) {

        print_node_asm(expression, node->left, output, node->data, LEFT_POS);
        print_node_asm(expression, node->right, output, node->data, RIGHT_POS);
    }

}

void print_var_asm(const MathExpression* expression, const Node* node, FILE* output) {

    switch(node->data) {
        case 0:
            fprintf(output, "rax\n");
            break;
        case 1:
            fprintf(output, "rbx\n");
            break;
        case 2:
            fprintf(output, "rcx\n");
            break;
        case 3:
            fprintf(output, "rdx\n");
            break;
        case 4:
            fprintf(output, "r1x\n");
            break;
        case 5:
            fprintf(output, "r2x\n");
            break;
    }
}
