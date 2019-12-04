#ifndef ___code_gen_h
#define ___code_gen_h

#include<stdio.h>

extern FILE *fout;

//四則演算のコード生成
void gen_binary(int token_kind);

//mov命令
void gen_mov(int imm);

//puch命令
void gen_push(int imm);

//構文木の走査
void compile(void);

//assembly prologue
void begin_compile(void);
//pop,ret fclose()
void end_compile(void);

#endif
