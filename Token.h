#ifndef ___token_gen_h
#define ___token_gen_h

#include<stdio.h>

/*������*/
// ��ʸ���Ⱦ�ʸ����Letter������
// _ �� Letter
// -(�ϥ��ե�) �� Minus ������
// Others �� �嵭ɽ�ˤʤ�ʸ���˳������Ƥ�ʸ����
// �����ֹ���Ascii�����ɤ˹��碌�ޤ�����
// String, NulKind, EofTkn, Ident�Υȡ��������ɲä��ޤ�����
// String �� ʸ�����ȡ�������
// EofTkn �� Eof�ѤΥȡ�������
// NulKind �� �����˻Ȥ����ȡ������Ǥ����������Ƥ��ȡ������郎�ʤ��Ȥ��˻Ȥ���

#define ID_SIZ 31
#define TEXT_SIZ 100

// トークンの種類
typedef enum{
  EofTkn,
  Digit=1,       Letter=2,    Others=3,
  String,        IntNum,      Ident,       NulKind,     EndList,
  Dollar='$',
  Lparen='(',    Rparen=')',  Lbrace='{',  Rbrace='}', Lbracket='[', Rbracket=']',
  Plus='+',      Minus='-',   Multi='*',   Divi='/',   Percent='%',  Assign='=',
  PlusEq=130,    MinusEq=131, MultiEq=132, DiviEq=133, PerEq=134,
  Less='<',      Great='>',   And='&',     Or='|',     Not='!',
  LessEq=135,    GreatEq=136, And2=137,    Or2=138,    NotEq=139, Equal=140,
  Dquot='"',     Squot='\'',  Comma=',',
  Semicolon=';', Colon=':',
  Dot='.',       Sharp='#',
  Newline='\n',  Tab='\t',    Space=' ',
  If=150,        Else=151,    While=152,  For=153,
}Kind;
// ���äƤʤ�ʸ�����黻�� \ ~ ^ @ ++ --

// トークン型
typedef struct{
  Kind kind;
  char text[TEXT_SIZ+1];
  int intVal;
} Token;

//�������Х��ѿ�
extern Kind chtyp[256];
extern FILE *fin;


//空白を無視して、次のトークンを読みだす関数
Token nextTkn(void);

//
void init_chtyp(void);

//次の文字を読み出す関数
int nextCh(void);

//���顼ʸ�����Ϥ��ƥץ�����������λ����
void err_exit(char* err_str);

int is_op2(char c1, char c2);
int is_reserved_wd(char *s);
Token set_kind(Token t);

#endif
