#ifndef ___token_gen_h
#define ___token_gen_h


/*注意点*/
// 大文字と小文字はLetterで統一
// _ は Letter
// -(ハイフン) は Minus に統合
// Others は 上記表にない文字に割り当てる文字種
// 配列番号はAsciiコードに合わせました。
// String, NulKind, EofTkn, Identのトークンを追加しました。
// String は 文字列トークン種
// EofTkn は Eof用のトークン種
// NulKind は 便利に使えるトークンです。あり当てるトークン種がないときに使う。

#define ID_SIZ 31
#define TEXT_SIZ 100

// 文字種 と トークン種
// 番号を指定していないのがトークン
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
//入ってない文字、演算子 \ ~ ^ @ && || <= >= != ++ -- *= /= += -= %=

// tokenデータ構造
typedef struct{
  Kind kind;
  char text[TEXT_SIZ+1];
  int intVal;
} Token;


extern char chtyp[256];
extern FILE* fin;


//ファイルからトークンを一つ取り出す
Token nextTkn(void);

//文字種表を初期化する
void init_chtyp(void);

//ファイルから1文字読み出す
int nextCh(void);

//エラー文を吐いてプログラムを終了する
void err_exit(char* err_str);

#endif
