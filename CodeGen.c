#include"Token.h"
#include"CodeGen.h"
#include<stdio.h>

FILE *fout;

// 二項演算子 (+-*/) 命令出力
// 引数 : トークンの種類(token_kind)
//
void gen_binary(int token_kind){
	
	char *op;
	
	switch(token_kind){
	case Plus:  op = "add"; break;
	case Minus: op = "sub"; break;
	case Multi: op = "imul"; break;
	case Divi:  op = "idiv"; break;
	default:
		printf("\"gen_binary\" error (%d)\n", token_kind);
		op = "miss";
		break;
	}

	fprintf(fout, "pop rbx\n\t");/*木構造の時は消す*/
	fprintf(fout, "pop rax\n\t");
	if(token_kind == Divi) {
		fprintf(fout, "mov edx, 0\n\t");
		fprintf(fout, "%s ebx\n\t", op);
		fprintf(fout, "push rax\n\t");/*木構造の時は消す*/
	} else {
		fprintf(fout, "%s eax, ebx\n\t", op);
		fprintf(fout, "push rax\n\t");/*木構造の時は消す*/
	}
}

//mov命令
void gen_mov(int imm){
	fprintf(fout, "mov rax, %d\n\t", imm);
}

void gen_push(int imm){
	fprintf(fout, "push %d\n\t", imm);
}

//コード生成関数
void compile(void){

	fprintf(fout,
			".intel_syntax noprefix\n"
			".text\n"
			".global _main\n"
			"_main:\n\t"
			"push\trbp\n\t"
			"mov rbp, rsp\n\t"
			"xor eax, eax\n\t");
}

//reave命令
void end_compile(void){
	fprintf(fout,
			"mov rsp, rbp\n\t"
			"pop rbp\n\t"
			"ret\n");

	fclose(fout);
}
