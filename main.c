#include<Token.h>
#include<Parser.h>
#include<CodeGen.h>

Kind chtyp[256];
FILE *fin;

Token token;
int errF;

FILE *fout;

int main(int argc, char *argv){
	if(argc == 1) exit(1);
	if((fin  = fopen(argv[1], "r")) == NULL) exit(1);
	if((fout = fopen(argv[2], "w")) == NULL) exit(1);

	init_chtyp();
	printf("text       kind intval\n");

	//構文解析 変更
	for(token = nextTkn(); token.kind != EofKind;){
		expression();
		if(errF) puts("---syntax error---");
	}
	//

	//アセンブリ生成 変更
	compile(node);
	//
	
	puts("end_compile");
	
	return 0;
}
