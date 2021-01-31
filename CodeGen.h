#ifndef ___code_gen_h
#define ___code_gen_h

#include<stdio.h>
#include"Parser.h"

extern FILE *fout;

//四則演算のコード生成
void gen_binary(int token_kind);

//構文木の走査
void pre_compile();
void compile(Node *node);

void Gen_expression(Node* node);

Kind lookLeftNode(Node* left);
#endif
