#include"Token.h"
#include"CodeGen.h"
#include<stdlib.h>


FILE *fout;

// $BFs9`1i;;;R(B (+-*/) $BL?Na=PNO(B
// $B0z?t(B : $B%H!<%/%s$N<oN`(B(token_kind)
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

//$B5<;w(BNode struct
//kind (token.kind)
//intVal
//*left
//*right
//$B9=J8LZAv::4X?t(B

// error
//  1 = 3 + a;
//  a + b = 1 + c;
//  a + 6;
// correct
//  a = 1 + b * c;

void pre_compile(){
	fprintf(fout,
			".intel_syntax noprefix\n"
			".text\n"
			".global _main\n"
			"_main:\n\t"
			"push\trbp\n\t"
			"mov rbp, rsp\n\t"
			"xor eax, eax\n\t");
	// "sub rbp, 12\n\t"
	
	//fprintf(fout, "mov dword ptr [rbp - 4], 0\n\t");
	//fprintf(fout, "mov dword ptr [rbp - 8], 0\n\t");
	//fprintf(fout, "mov dword ptr [rbp - 12], 0\n\t");
}

void compile(Node *node){
	Kind left;
	
	switch(node->kind){
	case '=':
		left = lookLeftNode(node->lhs);
		if(left != Ident) printf("left subtree is not Ident(%d)\n", left);
		
		fprintf(fout, "mov dword ptr [rbp - 4], 0\n\t");		/* $B:8JUCM(B */
		Gen_expression(node->rhs);
		fprintf(fout, "mov dword ptr [rbp - 4], eax\n\t");		/* $B1&JUCM(B */
		break;
		
	case '+': case '-': case '*': case '/':
		Gen_expression(node);
		break;
		
	case IntNum:
		fprintf(fout, "mov eax, %d\n\t", node->val);
		break;
		
	default:
		printf("Tree is not expression : (%d:%d)\n", node->kind, node->val);
		return;
	}
	
}

void Gen_expression(Node *node){
	static int IdCount = 0;
	
	switch(node->kind){
	case '+': case '-': case '*': case '/':
		Gen_expression(node->lhs);
		Gen_expression(node->rhs);
		gen_binary(node->kind);
		break;
		
	case IntNum:
		fprintf(fout, "mov eax, %d\n\t", node->val);
		fprintf(fout, "push rax\n\t");
		break;
		
	case Ident:
		IdCount++;
		
		if(IdCount > 3) {puts("Ident count is over"); exit(1);}
		
		fprintf(fout, "mov eax, dword ptr [rbp - %d]\n\t", IdCount*4 );
		fprintf(fout, "push rax\n\t");
		break;
		
	default:
		printf("syntax error (%d)\n", node->kind);
		exit(1);
	}
}

Kind lookLeftNode(Node* left){
	return left->kind;
}
