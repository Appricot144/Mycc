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
	
	fprintf(fout, "pop rbx\n\t");
	if(token_kind == Divi) {
		fprintf(fout, "mov edx, 0\n\t");
		fprintf(fout, "%s ebx\n\t", op);
	} else {
		fprintf(fout, "%s eax, ebx\n\t", op);
	}
}

//擬似Node struct
//kind (token.kind)
//intVal
//*left
//*right
//構文木走査関数

// error
//  1 = 3 + a;
//  a + b = 1 + c;
//  a + 6;
// correct
//  a = 1 + b * c;

void compile(Node *node){
		
	fprintf(fout,
			".intel_syntax noprefix\n"
			".text\n"
			".global _main\n"
			"_main:\n\t"
			"push\trbp\n\t"
			"mov rbp, rsp\n\t"
			"sub rbp, 12\n\t"
			"xor eax, eax\n\t");
	
	fprintf(fout, "mov dword ptr [rbp - 4], 0\n\t");
	fprintf(fout, "mov dword ptr [rbp - 8], 0\n\t");
	fprintf(fout, "mov dword ptr [rbp - 12], 0\n\t");
	
	switch(node->kind){
	case '=':
		int left;
		
		left = lookLeftNode(node->left);
		if(left != Ident) printf("left subtree is not Ident(%d)\n", left);
		
		fprintf(fout, "mov dword ptr [rbp - 4], 0\n\t");		/* 左辺値 */
		Gen_expression(node->right);
		fprintf(fout, "mov dword ptr [rbp - 4], eax\n\t");		/* 右辺値 */
		
	case '+': case '-': case '*': case '/':
		Gen_expression(node);

	case IntNum:
		fprintf(fout, "mov eax, %d\n\t", node->intVal);
		
	default:
		printf("Tree is not expression : (%d)\n", node->kind);
		return;
	}

	fprintf(fout,
			"mov rsp, rbp\n\t"
			"pop rbp\n\t"
			"ret\n");

	fclose(fout);
	
}

void Gen_expression(Node *node){
	static int IdCount = 1;
	
	switch(node->kind){
	case '+': case '-': case '*': case '/':
		Gen_expression(node->left);
		Gen_expression(node->right);
		gen_binary(node->kind);
		break;
		
	case IntNum:
		fprintf(fout, "mov eax, %d\n\t", node->intVal);
		fprintf(fout, "push eax\n\t");
		break;
		
	case Ident:
		IdCount++;
		
		if(IdCount > 3) {puts("Ident count is over"); exit(1);}
		
		fprintf(fout, "mov eax, dword ptr [rbp - %d]\n\t", IdCount*4 );
		fprintf(fout, "push eax\n\t");
		break;
		
	default:
		puts("syntax error (%d)", node->kind);
		exit(1);
	}
}
