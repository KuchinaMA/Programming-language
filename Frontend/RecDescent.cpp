#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <strings.h>

#include "Tree.h"
#include "RecDescent.h"
#include "TreeDump.h"
#include "ReadData.h"
#include "Diff.h"

CodeText* codedata_ctor(const char* str) {

    CodeText* data = (CodeText*)calloc(1, sizeof(CodeText));
    data->line = str;
    data->pointer = 0;

    return data;
}

void codedata_dtor(CodeText* text) {

    text->line = NULL;
    text->pointer = -1;

    free(text);
}

TokensArray* tokens_ctor() {

    TokensArray* tokens_arr = (TokensArray*)calloc(1, sizeof(TokensArray));
    tokens_arr->tokens_p = 0;

    return tokens_arr;
}

void tokens_dtor(TokensArray* arr) {

    for (int i = 0; i < arr->tokens_p; i++)
        node_dtor(arr->tokens[i]);

    arr->tokens_p = -1;

    free(arr);
}

Names* names_ctor() {

    Names* names = (Names*)calloc(1, sizeof(Names));
    names->names_num = KEYWORDS_NUM;
    names->vars_num = 0;
    fill_names_table(names->names_table);

    return names;
}

MathExpression* create_expression(const char* str, int size) {

    Names* names = names_ctor();
    CodeText* text = codedata_ctor(str);
    TokensArray* tokens_arr = tokens_ctor();

    string_to_tokens(text, size, tokens_arr, names);

    Node* root = get_grammar(tokens_arr);

    Tree* new_tree = tree_ctor(root);
    MathExpression* new_expression = expression_ctor(new_tree);
    copy_vars(new_expression, names);

    codedata_dtor(text);

    return new_expression;
}


Node* get_grammar(TokensArray* tokens_arr) {

    tokens_arr->tokens_p = 0;

    Node* node = get_operator(tokens_arr);

    if (node->type == T_OP && (node->data == 31 || node->data == 32))
        node = add_separator(node);

    Node* current = node;

    while ((tokens_arr->tokens[tokens_arr->tokens_p]->data == LINE_END) == 0) {

        Node* node2 = get_operator(tokens_arr);

        if (node2->type == T_OP && (node2->data == 31 || node2->data == 32))
            node2 = add_separator(node2);
        current->right = node2;
        current = node2;
    }
    assert(tokens_arr->tokens[tokens_arr->tokens_p]->data == LINE_END);

    return node;
}


Node* get_number(TokensArray* tokens_arr) {

    Node* node = tokens_arr->tokens[tokens_arr->tokens_p];
    tokens_arr->tokens_p++;

    return node;
}

Node* get_comparison(TokensArray* tokens_arr) {

    Node* node = get_expression(tokens_arr);
    while  (tokens_arr->tokens[tokens_arr->tokens_p]->type == T_OP &&
           (tokens_arr->tokens[tokens_arr->tokens_p]->data == MORE ||
            tokens_arr->tokens[tokens_arr->tokens_p]->data == LESS)) {

        Node* oper = tokens_arr->tokens[tokens_arr->tokens_p];
        tokens_arr->tokens_p++;

        Node* node2 = get_expression(tokens_arr);

        node = connect_nodes(oper, node, node2);
    }
    return node;
}



Node* get_expression(TokensArray* tokens_arr) {

    Node* node = get_term(tokens_arr);

    while  (tokens_arr->tokens[tokens_arr->tokens_p]->type == T_OP &&
           (tokens_arr->tokens[tokens_arr->tokens_p]->data == ADD ||
            tokens_arr->tokens[tokens_arr->tokens_p]->data == SUB)) {

        Node* oper = tokens_arr->tokens[tokens_arr->tokens_p];
        tokens_arr->tokens_p++;

        Node* node2 = get_term(tokens_arr);

        node = connect_nodes(oper, node, node2);
    }
    return node;
}


Node* get_term(TokensArray* tokens_arr) {

    Node* node = get_degree(tokens_arr);

    while  (tokens_arr->tokens[tokens_arr->tokens_p]->type == T_OP &&
           (tokens_arr->tokens[tokens_arr->tokens_p]->data == MUL ||
            tokens_arr->tokens[tokens_arr->tokens_p]->data == DIV)) {

        Node* oper = tokens_arr->tokens[tokens_arr->tokens_p];
        tokens_arr->tokens_p++;

        Node* node2 = get_degree(tokens_arr);

        node = connect_nodes(oper, node, node2);
    }
    return node;
}


Node* get_degree(TokensArray* tokens_arr) {

    Node* node = get_primary(tokens_arr);

    while (tokens_arr->tokens[tokens_arr->tokens_p]->type == T_OP &&
           tokens_arr->tokens[tokens_arr->tokens_p]->data == POW) {

        Node* oper = tokens_arr->tokens[tokens_arr->tokens_p];
        tokens_arr->tokens_p++;

        Node* node2 = get_primary(tokens_arr);

        node = connect_nodes(oper, node, node2);
    }
    return node;
}


Node* get_var(TokensArray* tokens_arr) {

    Node* var = tokens_arr->tokens[tokens_arr->tokens_p];
    tokens_arr->tokens_p++;

    return var;
}


Node* get_primary(TokensArray* tokens_arr) {

    if (tokens_arr->tokens[tokens_arr->tokens_p]->type == T_OP &&
        tokens_arr->tokens[tokens_arr->tokens_p]->data == OPEN_BR) {

        tokens_arr->tokens_p++;
        Node* node = get_expression(tokens_arr);

        assert(tokens_arr->tokens[tokens_arr->tokens_p]->type == T_OP &&
               tokens_arr->tokens[tokens_arr->tokens_p]->data == CLOSE_BR);
        tokens_arr->tokens_p++;
        return node;
    }

    else if (tokens_arr->tokens[tokens_arr->tokens_p]->type == T_NUM)
        return get_number(tokens_arr);

    else if (tokens_arr->tokens[tokens_arr->tokens_p]->type == T_VAR)
        return get_var(tokens_arr);

    else {

        Node* oper = tokens_arr->tokens[tokens_arr->tokens_p];
        tokens_arr->tokens_p++;

        if (tokens_arr->tokens[tokens_arr->tokens_p]->type == T_OP &&
            tokens_arr->tokens[tokens_arr->tokens_p]->data == OPEN_BR) {

            tokens_arr->tokens_p++;
            Node* node = get_expression(tokens_arr);

            assert(tokens_arr->tokens[tokens_arr->tokens_p]->type == T_OP &&
                   tokens_arr->tokens[tokens_arr->tokens_p]->data == CLOSE_BR);
            tokens_arr->tokens_p++;
            node = connect_nodes(oper, NULL, node);
            return node;
        }
    }
}

Node* get_assign(TokensArray* tokens_arr) {

    assert(tokens_arr->tokens[tokens_arr->tokens_p]->type == T_VAR);

    Node* node = get_var(tokens_arr);

    assert(tokens_arr->tokens[tokens_arr->tokens_p]->type == T_OP &&
           tokens_arr->tokens[tokens_arr->tokens_p]->data == EQUAL);

    Node* oper = tokens_arr->tokens[tokens_arr->tokens_p];
    tokens_arr->tokens_p++;

    Node* node2 = get_expression(tokens_arr);

    node = connect_nodes(oper, node, node2);

    Node* sep = tokens_arr->tokens[tokens_arr->tokens_p];
    tokens_arr->tokens_p++;

    connect_nodes(sep, node, NULL);

    return sep;
}

Node* get_in(TokensArray* tokens_arr) {

    Node* in_node = tokens_arr->tokens[tokens_arr->tokens_p];
    tokens_arr->tokens_p++;

    return in_node;
}

Node* get_if(TokensArray* tokens_arr) {

    assert(tokens_arr->tokens[tokens_arr->tokens_p]->type == T_OP &&
           tokens_arr->tokens[tokens_arr->tokens_p]->data == IF);
    Node* oper = tokens_arr->tokens[tokens_arr->tokens_p];
    tokens_arr->tokens_p++;

    assert(tokens_arr->tokens[tokens_arr->tokens_p]->type == T_OP &&
           tokens_arr->tokens[tokens_arr->tokens_p]->data == OPEN_BR);
    tokens_arr->tokens_p++;

    Node* node = get_comparison(tokens_arr);

    assert(tokens_arr->tokens[tokens_arr->tokens_p]->type == T_OP &&
           tokens_arr->tokens[tokens_arr->tokens_p]->data == CLOSE_BR);
    tokens_arr->tokens_p++;

    Node* node2 = get_operator(tokens_arr);

    connect_nodes(oper, node, node2);
    return oper;
}

Node* get_while(TokensArray* tokens_arr) {

    assert(tokens_arr->tokens[tokens_arr->tokens_p]->type == T_OP &&
           tokens_arr->tokens[tokens_arr->tokens_p]->data == WHILE);

    Node* oper = tokens_arr->tokens[tokens_arr->tokens_p];
    tokens_arr->tokens_p++;

    assert(tokens_arr->tokens[tokens_arr->tokens_p]->type == T_OP &&
           tokens_arr->tokens[tokens_arr->tokens_p]->data == OPEN_BR);
    tokens_arr->tokens_p++;

    Node* node = get_comparison(tokens_arr);

    assert(tokens_arr->tokens[tokens_arr->tokens_p]->type == T_OP &&
           tokens_arr->tokens[tokens_arr->tokens_p]->data == CLOSE_BR);
    tokens_arr->tokens_p++;

    Node* node2 = get_operator(tokens_arr);

    connect_nodes(oper, node, node2);
    return oper;

}

Node* get_operator(TokensArray* tokens_arr) {

    Node* res = node_ctor(T_OP, EQUAL, NULL, NULL);

    if (tokens_arr->tokens[tokens_arr->tokens_p]->type == T_OP &&
        tokens_arr->tokens[tokens_arr->tokens_p]->data == OPEN_F) {

        tokens_arr->tokens_p++;
        Node* node = get_operator(tokens_arr);
        Node* current = node;

        while ((tokens_arr->tokens[tokens_arr->tokens_p]->type == T_OP &&
                tokens_arr->tokens[tokens_arr->tokens_p]->data == CLOSE_F) == 0) {

            Node* node2 = get_operator(tokens_arr);
            current->right = node2;
            current = node->right;
        }

        assert(tokens_arr->tokens[tokens_arr->tokens_p]->type == T_OP &&
               tokens_arr->tokens[tokens_arr->tokens_p]->data == CLOSE_F);
        tokens_arr->tokens_p++;

        return node;
    }


    else if (tokens_arr->tokens[tokens_arr->tokens_p]->type == T_OP &&
        tokens_arr->tokens[tokens_arr->tokens_p]->data == IF)
        return get_if(tokens_arr);

    else if (tokens_arr->tokens[tokens_arr->tokens_p]->type == T_OP &&
             tokens_arr->tokens[tokens_arr->tokens_p]->data == WHILE)
        return get_while(tokens_arr);

    else
        return get_assign(tokens_arr);
}

int find_name(char* name, Names* names) {

    for (int i = 0; i < names->names_num; i++) {
        if (strcmp(name, names->names_table[i].name) == 0)
            return i;
    }
    return -1;
}

void fill_names_table(Name* table) {

    for (int i = 0; i < KEYWORDS_NUM; i++) {

        table[i].type = T_OP;
        table[i].number = 21 + i;
    }

    table[0].name = "плюс";
    table[1].name = "минус";
    table[2].name = "умножить_на";
    table[3].name = "делить_на";
    table[4].name = "в_степени";

    table[5].name = "ln";
    table[6].name = "sin";
    table[7].name = "cos";
    table[8].name = "tan";
    table[9].name = "ctg";

    table[10].name = "ежели";
    table[11].name = "покуда";

    table[12].name = "присвоить";
    table[13].name = "более";
    table[14].name = "менее";

    table[15].name = "ввести";
}

void copy_vars(MathExpression* new_exp, const Names* names) {

    assert(new_exp);

    for (size_t i = 0; i < names->vars_num; i++)
    {
        new_exp->variables_table[i].name  = strdup(names->names_table[KEYWORDS_NUM+i].name);
        new_exp->variables_table[i].value = names->names_table[KEYWORDS_NUM+i].number;
    }
}


void skip_spaces(CodeText* text) {

    while (text->line[text->pointer] == ' ' || text->line[text->pointer] == '\t'
        || text->line[text->pointer] == '\n' || text->line[text->pointer] == '\r')
        text->pointer++;
}


void string_to_tokens(CodeText* text, int size, TokensArray* tokens_arr, Names* names) {

    int i = 0;

    while(text->pointer < size) {

        switch(text->line[text->pointer]) {

            case ' ':
            case '\t':
            case '\r':
            case '\n': {

                skip_spaces(text);
                break;
            }

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': {

                tokens_arr->tokens[i] = read_number(text, tokens_arr);
                //printf("%d %d\n", tokens_arr->tokens[i]->type, tokens_arr->tokens[i]->data);
                i++;
                break;
            }

            case '+': {

                tokens_arr->tokens[i] = node_ctor(T_OP, ADD, NULL, NULL);
                text->pointer++;
                i++;
                break;
            }

            case '-': {

                tokens_arr->tokens[i] = node_ctor(T_OP, SUB, NULL, NULL);
                text->pointer++;
                i++;
                break;
            }

            case '*': {

                tokens_arr->tokens[i] = node_ctor(T_OP, MUL, NULL, NULL);
                text->pointer++;
                i++;
                break;
            }

            case '/': {

                tokens_arr->tokens[i] = node_ctor(T_OP, DIV, NULL, NULL);
                text->pointer++;
                i++;
                break;
            }

            case '^': {

                tokens_arr->tokens[i] = node_ctor(T_OP, POW, NULL, NULL);
                text->pointer++;
                i++;
                break;
            }

            case '=': {

                tokens_arr->tokens[i] = node_ctor(T_OP, EQUAL, NULL, NULL);
                text->pointer++;
                i++;
                break;
            }

            case '>': {

                tokens_arr->tokens[i] = node_ctor(T_OP, MORE, NULL, NULL);
                text->pointer++;
                i++;
                break;
            }

            case '<': {

                tokens_arr->tokens[i] = node_ctor(T_OP, LESS, NULL, NULL);
                text->pointer++;
                i++;
                break;
            }

            case '\0': {

                tokens_arr->tokens[i] = node_ctor(T_OP, LINE_END, NULL, NULL);
                text->pointer++;
                i++;
                break;
            }

            case '(': {

                tokens_arr->tokens[i] = node_ctor(T_OP, OPEN_BR, NULL, NULL);
                text->pointer++;
                i++;
                break;
            }

            case '!': {

                tokens_arr->tokens[i] = node_ctor(T_OP, SEP, NULL, NULL);
                text->pointer++;
                i++;
                break;
            }

            case ')': {

                tokens_arr->tokens[i] = node_ctor(T_OP, CLOSE_BR, NULL, NULL);
                text->pointer++;
                i++;
                break;
            }

            case '{': {

                tokens_arr->tokens[i] = node_ctor(T_OP, OPEN_F, NULL, NULL);
                text->pointer++;
                i++;
                break;
            }

            case '}': {

                tokens_arr->tokens[i] = node_ctor(T_OP, CLOSE_F, NULL, NULL);
                text->pointer++;
                i++;
                break;
            }

            default: {

                tokens_arr->tokens[i] = read_word(text, tokens_arr, names);
                i++;
            }

        }
    }
}

Node* read_word(CodeText* text, TokensArray* tokens_arr, Names* names) {

    char token[MAX_LINE_LEN] = "";
    int old_pointer = text->pointer;
    int i = 0;
    while (('a' <= text->line[text->pointer] && text->line[text->pointer] <= 'z') ||
           ('а' <= text->line[text->pointer] && text->line[text->pointer] <= 'я') ||
                   text->line[text->pointer] == '_') {
        token[i] = text->line[text->pointer];
        i++;
        text->pointer++;
    }

    int name = find_name(token, names);
    printf("token %s ", token);
    printf("name %d\n", name);

    if (name == -1) {
        names->names_table[names->names_num].name = strdup(token);
        names->names_table[names->names_num].number = names->vars_num;
        names->names_table[names->names_num].type = T_VAR;
        name = names->names_num;
        names->names_num ++;
        names->vars_num ++;
    }

    return node_ctor(names->names_table[name].type, names->names_table[name].number, NULL, NULL);
}

Node* read_number(CodeText* text, TokensArray* tokens_arr) {

    int value = 0;
    int old_pointer = text->pointer;
    while ('0' <= text->line[text->pointer] && text->line[text->pointer] <= '9') {
        value = value*10 + text->line[text->pointer] - '0';
        text->pointer++;
    }

    if (old_pointer < text->pointer)
        return node_ctor(T_NUM, value, NULL, NULL);
}


Node* add_separator(Node* node) {

    Node* sep = node_ctor(T_OP, SEP, NULL, NULL);

    connect_nodes(sep, node, NULL);
    return sep;
}





















