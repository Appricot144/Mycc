#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

#define ID_SIZ 31
#define TEXT_SIZ 100

// ʸ���� �� �ȡ������
// �ֹ����ꤷ�Ƥ��ʤ��Τ��ȡ�����
typedef enum{
  EofTkn,
  Digit=1, Letter=2, Others=3,
  String, IntNum, Ident, NulKind, 
  Dollar='$',
  Lparen='(', Rparen=')', Lbrace='{', Rbrace='}', Lbracket='[', Rbracket=']',
  Plus='+',   Minus='-',  Multi='*',  Divi='/',   Percent='%',  Assign='=',
  Less='<',   Great='>',  And='&',    Or='|',     Not='!',
  Dquot='"',  Squot='\'', Comma=',',
  Semicolon=';', Colon=':', 
  Dot='.',       Sharp='#',
  Newline='\n',  Tab='\t',  Space=' ',
}Kind;
//���äƤʤ�ʸ�����黻�� \ ~ ^ @ && || <= >= != ++ -- *= /= += -= %=

// token�ǡ�����¤
typedef struct{
  Kind kind;
  char text[TEXT_SIZ+1];
  int intVal;
} Token;

//�ե����뤫��ȡ�������ļ��Ф�
Token nextTkn(void);

//ʸ����ɽ����������
void init_chtyp(void);

//�ե����뤫��1ʸ���ɤ߽Ф�
int nextCh(void);

//���顼ʸ���Ǥ��ƥץ�����λ����
void err_exit(char* err_str);


Kind chtyp[256];
FILE* fin;


void init_chtyp(void){
  int i = 0;

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
// ��ǽ�� : �ե����뤫��ȡ�������ļ��Ф���
// ����� : Token
Token nextTkn(void){
  Token tkn = {NulKind, " ", 0};
  char *p = tkn.text, *p_31 = p+ID_SIZ, *p_100 = p+TEXT_SIZ;
  int num = 0, d = 0, cc = 0, err = 0;
  
  static int ch = ' ';
  
  while( isspace(ch) ){ ch = nextCh(); }
  if(ch == EOF){ tkn.kind = EofTkn; }

  switch(chtyp[ch]){

  case Letter: 
	for( ; chtyp[ch]==Letter || chtyp[ch]==Digit; ch = nextCh()){
	  if(p < p_31)
		*p++ = ch;
	}
	*p = '\0';
	//"for"��"if"�μ��̻Ҥ�ȡ���������Ͽ���Ƥ��ʤ��Τ�����Ident�ȡ�����ˤ��롣
	tkn.kind = Ident;
	break;

  case Digit:
	for(; chtyp[ch]==Digit; ch = nextCh(), d++ ){
	  num += (10*d) + (ch-'0');
	  *p++ = ch;
	}
	tkn.intVal = num;
	tkn.kind = IntNum;
	break;

  case Squot:
	for(ch=nextCh(); ch!=EOF && ch!='\n' && ch!='\''; ch=nextCh()){
	  *p++ = ch;
	  tkn.intVal = ch; 
	  cc++;
	  
	  if(ch == '\\') { /*����������ʸ���ν���*/
		ch = nextCh();
		if(ch=='n' && ch=='t'){ *p++ = ch; } else cc++;
	  }
	}
	if(cc >= 2){ err_exit("too many charactor(less than 2) (\') "); }
	if(ch==EOF || ch=='\n'){ err_exit("can\'t find Single Quotation(\')"); }

	tkn.kind = IntNum;
	break;

  case Dquot:
	for(ch=nextCh(); ch!=EOF && ch!='\n' && ch!='"'; ch=nextCh()){
	  if(p >= p_100){
		err = 1;
		break;
	  }else
		*p++ = ch;
	}
	*p = '\0';
	if(err != 0){ err_exit("too many charactor(less than 2) (\") "); }
	if(ch != '"'){ err_exit("can't find Double Quotation(\")"); }
	tkn.kind = String;
	break;

  default:/*�黻��*/
	*p = ch; *p='\0';
	tkn.kind = ch;
  }

  if(tkn.kind == Others) { 
	printf("iligal token (%s)\n", tkn.text);
	exit(1);
  }

  return tkn;
}

//nextCh
// ��ǽ�� : �ե����뤫��1ʸ���ɤ߽Ф�
// ����� : ���Ф���ʸ������(ascii��������)
// ���� ��: EOF�ΤȤ� 0 ���֤�
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
// ��ǽ  : ���顼ʸ����Ϥ��ƥץ�����λ���롣
void err_exit(char* err_str){
  puts(err_str);
  exit(1);
}

int main (void){
  init_chtyp();
  return 0;
}

