#include"Token.h"
#include"parser.h"
#include"CodeGen.h"
#include<stdlib.h>

Kind chtyp[256];
FILE *fin;

Token token;
int errF;

FILE *fout;

void expression(void) {
	Kind op;

	term();
	while (token.kind == Plus || token.kind == Minus) {
		op = token.kind;
		token = nextTkn();
		term();
		gen_binary(op);
		//â¡å∏éZââéZ
	}
}
void term(void) {
	Kind op;

	factor();
	while (token.kind == Multi || token.kind == Divi) {
		op = token.kind;
		token = nextTkn();
		factor();
		gen_binary(op);
		//èÊèúéZââéZ
	}
}

void factor(void) {
	switch (token.kind) {
	case IntNum:
		gen_mov(token.intVal);
		gen_push(token.intVal);
		break;
	case Lparen:
		token = nextTkn();
		expression();
		chkTkn(Rparen);
		break;
	default:
		if (token.kind == EofTkn) errF = 1;
	}
	token = nextTkn();
}

void chkTkn(Kind op) {
	if (token.kind != op) {
		errF = 1;
	}
}

int main (int argc, char *argv[]){
	if(argc == 1) exit(1);
	if((fin  = fopen(argv[1], "r")) == NULL) exit(1);
	if((fout = fopen(argv[2], "w")) == NULL) exit(1);
	
	init_chtyp();
	printf("text       kind intval\n");
	
	compile();

	for(token = nextTkn(); token.kind != EofTkn;){
		expression();
		if(errF) puts("---err---");
	}
	
	printf("end_compile\n");
	end_compile();

	return 0;
}

