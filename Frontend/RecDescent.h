#ifndef FUNCS_H_INCLUDED
#define FUNCS_H_INCLUDED

const int MAX_TOKENS_NUMB = 100;
const int KEYWORDS_NUM = 16;
const int MAX_NAMES_NUM = 100;

struct CodeText {
    const char* line;
    int pointer;
};

struct TokensArray {
    Node* tokens[MAX_TOKENS_NUMB];
    int tokens_p;
};

struct Names {
    Name names_table[MAX_NAMES_NUM];
    int names_num;
    int vars_num;
};

CodeText* codedata_ctor(const char* str);
void codedata_dtor(CodeText* text);

TokensArray* tokens_ctor();
void tokens_dtor(TokensArray* arr);

Names* names_ctor();
void names_dtor(Names* names);

MathExpression* create_expression(const char* str, int size);

Node* get_grammar(TokensArray* tokens_arr);

Node* get_number(TokensArray* tokens_arr);
Node* get_var(TokensArray* tokens_arr);
Node* get_in(TokensArray* tokens_arr);

Node* get_comparison(TokensArray* tokens_arr);
Node* get_expression(TokensArray* tokens_arr);
Node* get_term(TokensArray* tokens_arr);
Node* get_primary(TokensArray* tokens_arr);
Node* get_degree(TokensArray* tokens_arr);
Node* get_assign(TokensArray* tokens_arr);

Node* get_if(TokensArray* tokens_arr);
Node* get_while(TokensArray* tokens_arr);
Node* get_operator(TokensArray* tokens_arr);

void string_to_tokens(CodeText* text, int size, TokensArray* tokens_arr, Names* names);
Node* read_word(CodeText* text, TokensArray* tokens_arr, Names* names);
Node* read_number(CodeText* text, TokensArray* tokens_arr);
void skip_spaces(CodeText* text);

int find_name(char* name, Names* names);
void fill_names_table(Name* table);
void copy_vars(MathExpression* new_exp, const Names* names);

Node* add_separator(Node* node);

#endif // FUNCS_H_INCLUDED
