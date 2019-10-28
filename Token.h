#ifndef ___token_gen_h
#define ___token_gen_h


/*�����*/
// ��ʸ���Ⱦ�ʸ����Letter������
// _ �� Letter
// -(�ϥ��ե�) �� Minus ������
// Others �� �嵭ɽ�ˤʤ�ʸ���˳�����Ƥ�ʸ����
// �����ֹ��Ascii�����ɤ˹�碌�ޤ�����
// String, NulKind, EofTkn, Ident�Υȡ�������ɲä��ޤ�����
// String �� ʸ����ȡ������
// EofTkn �� Eof�ѤΥȡ������
// NulKind �� �����˻Ȥ���ȡ�����Ǥ����������Ƥ�ȡ�����郎�ʤ��Ȥ��˻Ȥ���

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


extern char chtyp[256];
extern FILE* fin;


//�ե����뤫��ȡ�������ļ��Ф�
Token nextTkn(void);

//ʸ����ɽ����������
void init_chtyp(void);

//�ե����뤫��1ʸ���ɤ߽Ф�
int nextCh(void);

//���顼ʸ���Ǥ��ƥץ�����λ����
void err_exit(char* err_str);

#endif
