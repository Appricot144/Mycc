#include<stdio.h>

#define TBL_MAX 256

typedef enum{
	varID, funcID, protFuncID,
} SymKind;

typedef struct{				/*									関数   	変数 	*/
	SymKind kind;  		 	/* 記号の種類					-		-		*/
	int type;						/* 型									|		|		*/
	char *name;					/* 記号の名前		 	  	|		|		*/
	int offset = 0;			/* rbpからのoffset			|		-		*/
	int arg_c;					/* 引数の数							|		 		*/
	int def_flag = 0;		/* 関数の定義の有無				|				*/
	int call_flag = 0;	/* 呼び出し(参照)の有無		-				*/
} Tuple;

typedef struct{
	int local_var_num = 0;
	int start_block = 0;
	int end_block = 0;
} BlkTbl_tuple;

Tuple SymTbl[TBL_MAX]; /* 記号表 */
int tbNo = 0;
Tuple temp;

//今見ているテーブルの領域管理変数
BlkTbl_tuple BlkTbl[TBL_NUM];
int scope = 0;

//***
//機能 :  記号表へ登録を行う関数
//	 以下の検知
//		1. 2重定義
//		2. 未定義を参照(または代入)
//		3. 関数名、変数名の被り
//引数
//  登録するIdentの名前
//  記号の種類(SymKind)
//戻り値
//  登録失敗時	: 0
//  成功時    	: 1
int entry(Tuple temp){

	switch(kind){
	case varID:

		if( search_name(temp.name) ){
			printf("Error : variable name colission (%s)\n", name);
			return 0;
		}

		local_var_num[scope]++;
		temp.offset = local_var_num[scope] * 4;

	case funcID:

		if( search_name(temp.name) ){
			return 0;
		}
		temp.type = type;
		temp.name = name;
		temp.arg_c = arg_c;


	case protFuncID:

		if( search_name(name,kind) ){
			printf("Error : function name colission (%s)\n", name);
			return 0;
		}
		temp.type = type;
		temp.name = name;
		temp.arg_c = arg_c;

	default:
		printf("Error : \'Kind\' (%d)\n", Kind);
	}

	SymTbl[tbNo] = temp;
	end_Block[scope] = ++tbNo;

	return 1;
}

//関数、変数の定義を発見した時に呼び出す。
void set_to_temp(SymKind kind, int type, char *name, int arg_c){
	temp.kind = kind;
	temp.type = type;
	temp.name = name;
	temp.arg_c = arg_c;

	entry(temp);
}

//Block構造の解析に入った時に呼び出す
void open_table(){
	scope++;
	tbNo++;
	BlkTbl[scope].start_block = tbNo;
	BlkTbl[scope].end_block = tbNo;
	BlkTbl[scope].local_var_num = 0;
}

//Block構造の解析を終了した時に呼び出す
void close_table(){
	scope--;
	tbNo = BlkTbl[scope].end_block;
}

//関数呼び出しがあったら呼ぶ関数
//指定の関数に呼び出しがあったことを記録する。
void set_callflag(char *name){
	int i;
	i = search_name(name);

	if(i == 0){
		printf("error : no exist function is called (%s)\n", name);
		return;
	}

	SymTbl[i].callflag = 1;
}

//関数の定義があったら呼ぶ関数
//指定の関数に定義が行われたことを記録する。
void set_defflag(char *name){
	if(scope != 0) {
		printf("error : 定義できない位置です. (%s)\n", name);
		return;
	}

	temp.defflag = 1;
}

//return
// colision == 1
// other 	== 0
//liner_search
int search_name(char *name){
	Tuple samp;

	for(int i=BlkTbl[scope].start_Block; i < BlkTbl[scope].end_Block; i++){
		samp = SybTbl[i];
		if( strcmp(name, samp.name) == 0  )
			return i;
	}

	return 0;
}

// テーブルからsymbolの情報を取り出す関数群

//上から変数を一つ取り出す関数
void get_tuple(){

}
