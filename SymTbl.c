#include"Token.h" /*TEXT_SIZのため*/
#include<stdio.h>

Tuple SymTbl[256];

typedef struct{
	char *name;
	int  offset;
	int  intval;
	int  
} Tuple;

void init_SymTbl(){
	for(int i=0; i<256; i++)
		SymTbl[i] = NULL;
}

void entry(char *name, int offset, int intVal){
	Tuple temp;

	if( search_name(name) )
		printf("variable \"%s\" is collision", name);
	
	temp.name = name;
	temp.offset = offset;
	temp.intVal = intVal;

	SymTbl[offset] = temp;
}

int get_tuple_value(int offset){
	return SymTbl[offset].intVal;
}

//return
// colision == 0
// other 	== 1
/*----------------*/
// ライナー探査
/*----------------*/
int search_name(char *name){
	Tuple temp;
	
	for(int i=0; i<256; i++){
		temp = SybTbl[i];
		if( strcmp(name, temp.name) == 0 )
			return 0;
	}

	return 1;
}
