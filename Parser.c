#include"Token.h"
#include"Parser.h"
#include"CodeGen.h"
#include<stdlib.h>

Kind chtyp[256];
FILE *fin;

Token token;
int errF;

FILE *fout;

Node* statement(void){
	Kind op;

	Node* node = expression();
	while(token.kind == '='){
		op = token.kind;
		token = nextTkn();
		node = new_node(op, node, expression());
	}
	return node;
}

Node* expression(void) {
	Kind op;

	Node* node = term();
	while (token.kind == Plus || token.kind == Minus) {
		op = token.kind;
		token = nextTkn();
		//////ここの条件設定が違う　nextTknしてからtoken.kindで条件分岐してしまっている
		//if (token.kind == Plus) {
			node = new_node(op,node,term());
			//}else if (token.kind == Minus) {
			//node = new_node(op,node,term());
			//}
	}
	return node;
}

Node* term(void) {
	Kind op;

	Node* node = factor();
	while (token.kind == Multi || token.kind == Divi) {
		op = token.kind;
		token = nextTkn();
		//if (token.kind == Multi) {
			node = new_node(op, node, factor());
			//}else if (token.kind == Divi) {
			//node = new_node(op, node, factor());
			//}
	}
	return node;
}

Node* factor(void) {
	Node* node;
	switch (token.kind) {
	case IntNum: //整数
		node = new_node_num(token.kind,token.intVal);
		token = nextTkn();
		return node;
	case Lparen:
		token = nextTkn();
		node = expression();
		chkTkn(Rparen);
		token = nextTkn();
		return node;
	case Ident: //変数
		node = new_node_num(token.kind, token.intVal);
		token = nextTkn();
		return node;
	default:
		return NULL;
	}
}

void chkTkn(Kind op) {
	if (token.kind != op) {
		errF = 1;
	}
}

Node* new_node(Kind kind, Node* lhs, Node* rhs) {
	Node* node = (Node*)calloc(1, sizeof(Node));
	node->kind = kind;
	node->lhs = lhs;
	node->rhs = rhs;
	return node;
}

Node* new_node_num(Kind kind, int val) {
	Node* node = (Node*)calloc(1, sizeof(Node));
	node->kind = kind;
	node->val = val;
	return node;
}

void printNode(Node* node) {
	printf("now.kind  : %d\n", node->kind);
	printf("now.left  : %p\n", node->lhs);
	printf("now.right : %p\n", node->rhs);
	if (node->kind == IntNum) {
		printf("now.val : %d\n", node->val);
		puts("IntNum");
	}

	switch (node->kind) {
	case Plus:
		printf("Plus\n");
		break;
	case Minus:
		printf("Minus\n");
		break;
	case Multi:
		printf("Multi\n");
		break;
	case Divi:
		printf("Divi\n");
		break;
	case '=':
		printf("equal\n");
		break;
	case Ident://変数
		printf("variable\n");
		break;
	case Lparen:
		printf("Lparen\n");
		break;
	case Rparen:
		printf("Rparen\n");
		break;
	case NulKind:
		break;
	default:
		break;
	}
	
	if(node->lhs == 0) return;
	puts("-------------into left--------------");
	printNode(node->lhs);
	
	if(node->rhs == 0) return;
	puts("-------------into right-------------");
	printNode(node->rhs);
	puts("------------------------------------");
}


int main (int argc, char *argv[]){
	if(argc == 1) exit(1);
	if((fin  = fopen(argv[1], "r")) == NULL) exit(1);
	if((fout = fopen(argv[2], "w")) == NULL) exit(1);
	
	Node* node;

	init_chtyp();
	// nextTkn()動作テスト用 
	printf("text       kind intval\n");

	// .intel_syntax noprefix
	// _main
	pre_compile();
	// [要修正] : 1ループ毎にnextTkn()で';'を読み飛ばしている
	for(token = nextTkn(); token.kind != EofTkn; token = nextTkn()){
		node = statement();
		if(errF) puts("---err---");
		// statement()動作テスト用
		printNode(node);
		
		printf("==========now.node.kind : %d=============\n", node->kind);
		compile(node);
	}

	fprintf(fout,
			"mov rsp, rbp\n\t"
			"pop rbp\n\t"
			"ret\n");
	
	printf("end_compile\n");
	fclose(fout);

	return 0;
}
