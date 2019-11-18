#include"Token.h"
#include<string.h>
#include<stdlib.h>
#include<ctype.h>


Kind chtyp[256];
FILE *fin;

// 文字列とトークンの対応
struct{
  char *keywd_text;
  Kind kkind;
} KeywdTbl[] = {
  {"+=",	PlusEq }, {"-=",   MinusEq},
  {"*=",	MultiEq}, {"/=",   DiviEq },
  {"<=",    LessEq }, {">=",   GreatEq}, 
  {"&&",    And2   }, {"||",   Or2    },
  {"!=",    NotEq  }, {"==",   Equal  },
  {"if",    If     }, {"else", Else   },
  {"while", While  }, {"for",  For    },
  {"", 		EndList}
};

void init_chtyp(void){
	int i = 0;

	for(i=0; i<256; i++){chtyp[i] = Others;}
	for(i='a'; i<'z'; i++){chtyp[i] = Letter; }
	for(i='A'; i<'Z'; i++){chtyp[i] = Letter; }
	for(i='0'; i<'9'; i++){chtyp[i] = Digit;  }
	chtyp['_'] = Letter;
	chtyp['$'] = Dollar;
	chtyp['('] = Lparen;   chtyp[')'] = Rparen; 
	chtyp['{'] = Lbrace;   chtyp['}'] = Rbrace; 
	chtyp['['] = Lbracket; chtyp[']'] = Rbracket;
	chtyp['+'] = Plus;     chtyp['-'] = Minus; 
	chtyp['*'] = Multi;    chtyp['/'] = Divi; 
	chtyp['%'] = Percent;  chtyp['='] = Assign; 
	chtyp['<'] = Less;     chtyp['>'] = Great; 
	chtyp['&'] = And;      chtyp['|'] = Or;         chtyp['!'] = Not;
	chtyp['"'] = Dquot;    chtyp['\''] = Squot; 
	chtyp[','] = Comma;    chtyp[';'] = Semicolon;  chtyp[':'] = Colon;
	chtyp['.'] = Dot;      chtyp['#'] = Sharp;
	chtyp['\n'] = Newline; chtyp['\t'] = Tab; 
	chtyp[' '] = Space;
}

//nextTkn
// 機能　 : ファイルからトークンを一つ取り出す。
// 戻り値 : Token
Token nextTkn(void){
	Token tkn = {NulKind, " ", 0};
	char *p = tkn.text, *p_31 = p+ID_SIZ, *p_100 = p+TEXT_SIZ;
	int num = 0, cc = 0, err = 0;
	
	static int ch = ' ';
  
	while( isspace(ch) ){ ch = nextCh(); }
	if(ch == EOF){ tkn.kind = EofTkn; return tkn;}

	switch(chtyp[ch]){
		
	case Letter:
		for( ; chtyp[ch]==Letter || chtyp[ch]==Digit; ch = nextCh()){
			if(p < p_31)
				*p++ = ch;
		}
		*p = '\0';
		break;

	case Digit:
		for(; chtyp[ch]==Digit; ch = nextCh()){
			num = (10*num) + (ch-'0');
			*p++ = ch;
		}
		tkn.intVal = num;
		tkn.kind = IntNum;
		break;

	case Squot:
		for(ch = nextCh(); ch!=EOF && ch!='\n' && ch!='\''; ch=nextCh() ){
			*p++ = ch;
			tkn.intVal = ch; 
			cc++;
			
			if(ch == '\\') { /*エスケープ文字の処理*/
				ch = nextCh();
				if(ch=='n' || ch=='t') *p++ = ch;
				else cc++;
			}
		}
		if(cc >= 2){ err_exit("too many charactor(less than 2) (\') "); }
		if(ch==EOF || ch=='\n'){ err_exit("can\'t find Single Quotation(\')"); }
		ch = nextCh(); /*閉じている'を捨てる*/
	  
		tkn.kind = IntNum;
		break;

	case Dquot:
		for(ch = nextCh(); ch != EOF && ch != '\n' && ch!='"'; ch=nextCh()){
			if(p >= p_100) err = 1;
			else *p++ = ch;
		}
		*p = '\0';
		if(err != 0){ err_exit("too many charactor(less than 100) (\") "); }
		if(ch != '"'){ err_exit("can't find Double Quotation(\")\n"); }

		ch = nextCh(); /*閉じている"を捨てる*/
		tkn.kind = String;
		break;

	default:/*演算子, その他のトークン設定*/
		*p++ = ch;
		ch = nextCh();
		if(is_op2(*(p-1), ch)) { /*op2*/
			*p++ = ch; 
			ch = nextCh();
			*p = '\0';
		}else{ 					 /*op1*/
			*p = '\0'; 
		}

	}
	if(tkn.kind == NulKind) { tkn = set_kind(tkn);}
	if(tkn.kind == Others ) { 
		printf("iligal token (%s)\n", tkn.text);
		exit(1);
	}

	printf("%-10s %4d %6d\n",tkn.text,tkn.kind,tkn.intVal);//

	return tkn;
}

int is_op2(char c1,char c2){
	char s[] = "--";
	s[0] = c1, s[1] = c2;
	return strstr(" +=,-=,*=,/=,<=,>=,==,!=,&&,||, ", s) != NULL;
}

Token set_kind(Token t){
	char *s = t.text;
	int i;
	for(i=0; KeywdTbl[i].kkind != EndList; i++){ //op2 & reserved_words
		if(strcmp(KeywdTbl[i].keywd_text, s) == 0){
			t.kind = KeywdTbl[i].kkind;
			return t;
		}
	}
	if(chtyp[*s]==Letter){ //Ident
		t.kind = Ident;
	}else { 				 //op1
		t.kind = chtyp[*s];
	}
	return t;
}

//nextCh
// 機能　 : ファイルから1文字読み出す
// 戻り値 : 取り出した文字の値(asciiコード値)
// 　　 　: EOFのとき 0 を返す
int nextCh(void){
	static int ch;
	ch = fgetc(fin);
	if(ch == EOF){
		fclose(fin);
		return ch;
	}
	return ch;
}

//err_exit
// 機能  : エラー文を出力してプログラムを終了する。
void err_exit(char* err_str){
	puts(err_str);
	exit(1);
}

/*
int main (int argc, char *argv[]){
	if(argc == 1) exit(1);
	if((fin = fopen(argv[1], "r")) == NULL) exit(1);
	Token token;
	int i;

	printf("text       kind intVal\n");
	init_chtyp();
	for(i=0; i<256; i++){
		token = nextTkn();
		if(token.kind == EofTkn) break;
		printf("%-10s %4d %6d\n", token.text, token.kind, token.intVal);
	}
	return 0;
}
*/
